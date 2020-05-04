#ifndef FOLDABLE_SCREEN_H
#define FOLDABLE_SCREEN_H

#pragma once
#include <key-controller.h>
#include <screen-controller.h>
#include <demo-web-server.h>
#include <FS.h>

class FoldableScreen
{
    private:
        KeyController _upKey;
        KeyController _downKey;
        ScreenController & _screenController;
        DemoWebServer & _demoWebServer;

    public:
        FoldableScreen(Scheduler& ts, FS& fs, ConfigProvider& configProvider, StateBlinker & blinker,
ScreenController & screenController, DemoWebServer & demoWebServer);
        //TODO: hardware keys
};

#endif