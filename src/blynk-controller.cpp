#include <blynk-controller.h>

#include <ArduinoLog.h>

#ifdef ENABLE_BLYNK
#include <BlynkSimpleEsp8266.h>
#include <delays.h>

extern BlynkController blynkController;

BLYNK_CONNECTED(){
    blynkController.UpdateAppState();
}

BLYNK_WRITE(V0){
    int val = param.asInt();
    Log.notice("From blynk %d\n", val);
    blynkController.In2(val);
}
#endif

//TODO: extract WiFi control from blynk
BlynkController::BlynkController(ScreenController & screenController,
 ConfigProvider & configProvider,
 Scheduler & ts)
: _screenController(screenController)
, runTask(BLYNK_DELAY, TASK_FOREVER,
    std::bind(&BlynkController::run, this) 
    , &ts),
auth(configProvider.GetBlynkAuth())
{
    handler1 = WiFi.onStationModeGotIP([&configProvider, this](const WiFiEventStationModeGotIP& event){
    #ifdef ENABLE_BLYNK
        if(!isInitiated)
        {
            Blynk.config(auth.c_str());
            isInitiated = true;
        }
    #endif
        runTask.restart();
    });
    handler2 = WiFi.onStationModeDisconnected([this](const WiFiEventStationModeDisconnected& event)
    {
        runTask.disable();
    });
    handler3 = _screenController.onStateChanged([this](){
        UpdateAppState();
    });
}

void BlynkController::run()
{
#ifdef ENABLE_BLYNK
    Blynk.run();
#endif
}

void BlynkController::UpdateAppState()
{
    int moveState = _screenController.getMoveState();
    int position = _screenController.getPosition();
    Log.notice("Updating blink state.  move: %d, pos: %d\n", moveState, position);
    Blynk.virtualWrite(V0, moveState);
    Blynk.virtualWrite(V1, position);
}

enum VIRTUAL_PIN_DIR {VIRTUAL_PIN_DIR_UP=1, VIRTUAL_PIN_DIR_STOP=2, VIRTUAL_PIN_DIR_DOWN=3};

void BlynkController::In2(int value)
{
    switch (value)
    {
    case VIRTUAL_PIN_DIR_UP:
        _screenController.roll(Direction::UP);
        break;
    case VIRTUAL_PIN_DIR_DOWN:
        _screenController.roll(Direction::DOWN);
        break;
    case VIRTUAL_PIN_DIR_STOP:
        _screenController.stop();
        break;
    default:
        Log.warning("Invalid  input from blynk %d", value);
        break;
    }
}