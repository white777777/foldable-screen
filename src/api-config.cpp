#include <api-config.h>

ApiConfigController::ApiConfigController(AsyncWebServer& _server, ConfigProvider& configProvider)
: _configProvider(configProvider)
{
    _server.addHandler(new AsyncCallbackJsonWebHandler("/api/config", [this](AsyncWebServerRequest *request, const JsonVariant &json){
        const JsonObject& jsonObj = json.as<JsonObject>();
        SetConfigFromJson(jsonObj, "up_time", CONFIG_NODES::PULL_UP_TIME);
        SetConfigFromJson(jsonObj, "down_time", CONFIG_NODES::PULL_DOWN_TIME);
        SetConfigFromJson(jsonObj, "ftp_enabled", CONFIG_NODES::FTP_ENABLED);
        SetConfigFromJson(jsonObj, "ota_enabled", CONFIG_NODES::OTA_ENABLED);
        SetConfigFromJson(jsonObj, "screen_position", CONFIG_NODES::SCREEN_POSITION);
        request->send(200);
    }));

    _server.on("/api/config", HTTP_GET, [this](AsyncWebServerRequest* request){
        AsyncResponseStream *response = request->beginResponseStream("application/json");

        DynamicJsonDocument root(1024);
        root["heap"] = ESP.getFreeHeap();
        root["ssid"] = WiFi.SSID();
        root["up_time"] = _configProvider.Get(CONFIG_NODES::PULL_UP_TIME);
        root["down_time"] = _configProvider.Get(CONFIG_NODES::PULL_DOWN_TIME);
        root["ftp_enabled"] = _configProvider.Get(CONFIG_NODES::FTP_ENABLED);
        root["ota_enabled"] = _configProvider.Get(CONFIG_NODES::OTA_ENABLED);
        root["screen_position"] = _configProvider.Get(CONFIG_NODES::SCREEN_POSITION);
        serializeJson(root, *response);
        request->send(response);
    });
}

void ApiConfigController::SetConfigFromJson(const JsonObject& jsonObj, const String& name, const CONFIG_NODES node)
{
    if(jsonObj.containsKey(name))
    {
        auto value = jsonObj[name].as<uint8_t>();
        _configProvider.Set(node, value);
    }
}