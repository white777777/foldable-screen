#include <foldable-screen.h>

FoldableScreen::FoldableScreen(Scheduler &ts, FS& fs, ConfigProvider& configProvider, StateBlinker & blinker,
ScreenController & screenController, DemoWebServer & demoWebServer)
: _upKey(ts)
, _downKey(ts)
, _screenController(screenController)
, _demoWebServer(demoWebServer)
{
    _upKey.SetCallback([this](KeyController& k, bool isPressed){
        if(isPressed)
            this->_screenController.roll(Direction::UP);
        else
            this->_screenController.stop();
    });
    _downKey.SetCallback([this](KeyController& k, bool isPressed){
        if(isPressed)
            this->_screenController.roll(Direction::DOWN);
        else
            this->_screenController.stop();
    });
}