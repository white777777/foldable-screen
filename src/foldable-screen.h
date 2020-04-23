#ifndef FOLDABLE_SCREEN_H
#define FOLDABLE_SCREEN_H

#include <key-controller.h>
#include <screen-controller.h>
#include <demo-web-server.h>
#include <FS.h>

class FoldableScreen
{
    private:
        KeyController _upKey;
        KeyController _downKey;
        ScreenController _screenController;
        DemoWebServer _demoWebServer;

    public:
        FoldableScreen(Scheduler& ts, FS& fs, ConfigProvider& configProvider, StateBlinker & blinker);
        //TODO: hardware keys
        //TODO: connect with rest
};

#endif