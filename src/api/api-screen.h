#pragma once
#include <ESPAsyncWebServer.h>
#include <screen-controller.h>

class ApiScreenController
{
    private:
        ScreenController & _screenController;
    public:
        ApiScreenController(AsyncWebServer & server, ScreenController & screenController);
};