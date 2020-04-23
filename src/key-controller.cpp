#include <key-controller.h>
#include <Arduino.h>

//TODO: short-long press logic
void KeyController::PressKey(int timeout){
    if(!_isKeyPressed)
    {
        //notify is state changes
        _isKeyPressed = true;
        _callback(*this, _isKeyPressed);
    }
    _checkTimeoutTask.restartDelayed(timeout);
}

void KeyController::ReleaseKey(){
    _checkTimeoutTask.disable();
    if(_isKeyPressed)
    {
        _isKeyPressed = false;
        _callback(*this, _isKeyPressed);
    }
}

bool KeyController::IsKeyPressed(){
    return _isKeyPressed;
}

void KeyController::SetCallback(std::function<void(KeyController&, bool)> callback){
    if(callback == nullptr)
        _callback = [](KeyController& a, bool b){};
    else
        _callback = callback;
}

KeyController::KeyController(Scheduler& ts): 
_ts(ts),
_checkTimeoutTask(0, 1)
{
    _checkTimeoutTask.setCallback([&]{
        ReleaseKey();
    });
    _ts.addTask(_checkTimeoutTask);
    //TODO: destructor: _ts.deleteTask(_checkTimeoutTask);
}