#pragma once
#include <ESPAsyncWebServer.h>
#include <screen-controller.h>
#include <FS.h>
#include <config-provider.h>
#include <api-config.h>
#include <api-files.h>
#include <api-legacy.h>
#include <api-status.h>
#include <api-screen.h>

class DemoWebServer
{
    private:
        AsyncWebServer _server = AsyncWebServer(80);
        ApiScreenController _apiScreenController;
        ApiConfigController _apiConfigController;
        LegacyApiController _apiLegacyController;
        ApiStatusController _apiStatusController;
        ApiFilesController _apiFilesController;

    public:
        DemoWebServer(ScreenController & screenController, FS & fs, ConfigProvider & configProvider);
};