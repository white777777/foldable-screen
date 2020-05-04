#include <screen-controller.h>
#include <Arduino.h>
#include <ArduinoLog.h>

constexpr int screenPosMax = 200;
constexpr int screenPosMin = 0;
constexpr uint8_t timeMultiplier = 100;
ScreenController::ScreenController(Scheduler & ts, ConfigProvider& configProvider, StateBlinker & blinker):
stopRollTask(TASK_IMMEDIATE, TASK_ONCE, std::bind(&ScreenController::stop2, this), &ts),
startRollTask(TASK_IMMEDIATE, TASK_ONCE, __null, &ts),
notifyTask(1000*TASK_MILLISECOND, TASK_FOREVER, std::bind(&ScreenController::_eventCallback, this), &ts),
_config(configProvider),
_blinker(blinker)
{
    pinMode(pinUp, OUTPUT);
    pinMode(pinDown, OUTPUT);
    digitalWrite(pinUp, HIGH);
    digitalWrite(pinDown, HIGH);
}

uint8_t ScreenController::DirectionToPin(Direction direction)
{
    if(direction == Direction::UP)
        return pinUp;
    else if(direction == Direction::DOWN)
        return pinDown;
    else
        return undefinedPin;
}

unsigned long ScreenController::DirectionToTime(Direction direction)
{
    if(direction == Direction::UP)
        return timeMultiplier*_config.Get(CONFIG_NODES::PULL_UP_TIME)*TASK_MILLISECOND ;
    else if (direction == Direction::DOWN)
        return timeMultiplier*_config.Get(CONFIG_NODES::PULL_DOWN_TIME)*TASK_MILLISECOND;
    else
        return 0;
}

void ScreenController::roll(Direction direction, unsigned long time){
    roll(DirectionToPin(direction), time);
}

void ScreenController::roll(Direction direction){
    roll(DirectionToPin(direction), DirectionToTime(direction));
}

void ScreenController::roll(uint8_t pin, unsigned long time)
{
    if(_currentPin != undefinedPin)
        return;
    startRollTask.setCallback([this, pin, time](){
    
        Log.notice("screen roll\n");

        _currentPin = pin;
        _fullMoveTime = time;
        //_eventCallback();

        _blinker.SetDelay(500*TASK_MILLISECOND);

        const int position = _config.Get(SCREEN_POSITION);
        //const int position = calcPosition();

        unsigned long partialMoveTime;
        if(_currentPin == pinDown)
            //down
            partialMoveTime = _fullMoveTime - (position-screenPosMin)*_fullMoveTime/(screenPosMax -screenPosMin);
        else
            //up
            partialMoveTime = (position-screenPosMin)*_fullMoveTime/(screenPosMax -screenPosMin);

        _startTime = millis();
        digitalWrite(_currentPin, LOW);
        stopRollTask.restartDelayed(partialMoveTime);
        notifyTask.restart();
    });
    startRollTask.restart();
}

void ScreenController::stop(){
    stopRollTask.restart();
}

uint8_t ScreenController::calcPosition()
{
    const int prevPosition = _config.Get(SCREEN_POSITION);
    if(_currentPin == undefinedPin || _fullMoveTime == 0)
        return prevPosition;

    const auto moveTime = millis() - _startTime;
    const int movedLength = moveTime*(screenPosMax-screenPosMin)/_fullMoveTime;
    int position = prevPosition;
    if(_currentPin == pinDown)
    {
    //down
        position = prevPosition + movedLength;
    }
    else if(_currentPin == pinUp)
    {
        //up
        position = prevPosition - movedLength;
    }
    if(position < screenPosMin)
    {
        Log.warning("Position exceeds lower limits. Value: %d\n", position);
        position = screenPosMin;
    }
    if(position >screenPosMax)
    {
        Log.warning("Position exceeds higher limits. Value: %d\n", position);
        position = screenPosMax;
    }
    return position;
}

void ScreenController::stop2(){
    if(_currentPin == undefinedPin)
        return;

    digitalWrite(_currentPin, HIGH);

    notifyTask.disable();
    _blinker.TurnOff();
    Log.notice("screen stop\n");

    int position = calcPosition();
    _config.Set(SCREEN_POSITION, (uint8_t) position);

    _currentPin = undefinedPin;
    _eventCallback();
}

MOVE_STATE ScreenController::getMoveState()
{
    if(_currentPin == undefinedPin)
        return MOVE_STOP;
    if(_currentPin == pinDown)
        return MOVE_DOWN;
    if(_currentPin == pinUp)
        return MOVE_UP;
    return INVALID;
}

uint8_t ScreenController::getPosition()
{
    return calcPosition();
}

ScreenEventHandler ScreenController::onStateChanged(std::function<void()> f)
{
    ScreenEventHandler handler = std::make_shared<std::function<void()>>(f);
    sCbEventList.push_back(handler);
    return handler;
}

void ScreenController::_eventCallback()
{
    for(auto it = std::begin(sCbEventList); it != std::end(sCbEventList); ) {
        auto &handler = *it;
        if (handler.unique()) {
            it = sCbEventList.erase(it);
        }
        else {
            (*handler)();
            ++it;
        }
    }
}