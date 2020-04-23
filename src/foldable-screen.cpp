#include <foldable-screen.h>

FoldableScreen::FoldableScreen(Scheduler &ts, FS& fs, ConfigProvider& configProvider, StateBlinker & blinker)
: _upKey(ts)
, _downKey(ts)
, _screenController(ts, configProvider, blinker)
, _demoWebServer(_screenController, fs, configProvider)
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