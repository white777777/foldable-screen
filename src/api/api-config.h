#pragma once

#include <ESPAsyncWebServer.h>
#include "AsyncJson.h"
#include "ArduinoJson.h"
#include <config-provider.h>

class ApiConfigController
{
    private:
        ConfigProvider& _configProvider;
    private:
        void SetConfigFromJson(const JsonObject& jsonObj, const String& name, const CONFIG_NODES node);
    public:
        ApiConfigController(AsyncWebServer& _server, ConfigProvider& configProvider);
};