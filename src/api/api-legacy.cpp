#include <api-legacy.h>


LegacyApiController::LegacyApiController(AsyncWebServer& _server, ScreenController& screenController)
: _screenController(screenController)
{
    _server.on("/up", HTTP_GET, [this](AsyncWebServerRequest* request){
        _screenController.roll(Direction::UP);
        request->send(200);
    });
    _server.on("/down", HTTP_GET, [this](AsyncWebServerRequest* request){
        _screenController.roll(Direction::DOWN);
        request->send(200);
    });
        _server.on("/up1s", HTTP_GET, [this](AsyncWebServerRequest* request){
        _screenController.roll(Direction::UP, 1000);
        request->send(200);
    });
    _server.on("/down1s", HTTP_GET, [this](AsyncWebServerRequest* request){
        _screenController.roll(Direction::DOWN, 1000);
        request->send(200);
    });
}