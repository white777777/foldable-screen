#include <api-status.h>
#include <ArduinoJson.h>

ApiStatusController::ApiStatusController(AsyncWebServer & server)
{
    server.on("/api/status", HTTP_GET, [this](AsyncWebServerRequest* request){
        AsyncResponseStream *response = request->beginResponseStream("application/json");

        DynamicJsonDocument root(1024);
        root["heap"] = ESP.getFreeHeap();
        root["ssid"] = WiFi.SSID();
        serializeJson(root, *response);
        request->send(response);
    });
}