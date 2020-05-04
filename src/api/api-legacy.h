#pragma once
#include <ESPAsyncWebServer.h>
#include <screen-controller.h>

class LegacyApiController
{
    private:
        ScreenController & _screenController;
    public:
        LegacyApiController(AsyncWebServer& server, ScreenController& screenController);
};
