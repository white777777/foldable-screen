#include <screen-controller.h>
#include <Arduino.h>
#include <ArduinoLog.h>

const uint8_t screenPosMax = 200;
const uint8_t timeMultiplier = 100;
ScreenController::ScreenController(Scheduler & ts, ConfigProvider& configProvider, StateBlinker & blinker):
ts(ts),
stopRollTask(0,1),
startRollTask(0,1),
_config(configProvider),
_blinker(blinker)
{
    pinMode(pinUp, OUTPUT);
    pinMode(pinDown, OUTPUT);
    digitalWrite(pinUp, HIGH);
    digitalWrite(pinDown, HIGH);
    ts.addTask(stopRollTask);
    ts.addTask(startRollTask);
    
    stopRollTask.setCallback([this](){
        stop2();
    });
}

uint8_t ScreenController::DirectionToPin(Direction direction)
{
    if(direction == Direction::UP)
        return pinUp;
    else if(direction == Direction::DOWN)
        return pinDown;
    else
        return 0;
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
    if(_currentPin != 0)
        return;
    
    startRollTask.setCallback([this, pin, time](){
        Log.notice("screen roll");

        _zzzTime = time;
        _startTime = millis();
        _currentPin = pin;
        _blinker.SetDelay(500*TASK_MILLISECOND);
        digitalWrite(_currentPin, LOW);

        unsigned long t;
        auto position = _config.Get(SCREEN_POSITION);
        if(_currentPin == pinDown)
            //down
            t = _zzzTime - position*_zzzTime/screenPosMax;
        else
            //up
            t = position*_zzzTime/screenPosMax;
        stopRollTask.restartDelayed(t);
    });
    startRollTask.restart();
}

void ScreenController::stop(){
    stopRollTask.restart();
}

void ScreenController::stop2(){
    if(_currentPin == 0)
        return;

    _blinker.TurnOff();
    Log.notice("screen stop");

    auto moveTime = millis() - _startTime;
    auto position = _config.Get(SCREEN_POSITION);
    if(_currentPin == pinDown)
    {
    //down
        position = position + moveTime*screenPosMax/_zzzTime;
    }
    else
    {
        //up
        position = position - moveTime*screenPosMax/_zzzTime;
    }
    _config.Set(SCREEN_POSITION, position);

    digitalWrite(_currentPin, HIGH);
    _currentPin = 0;
}
