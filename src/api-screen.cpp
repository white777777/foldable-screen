#include <api-screen.h>
#include "AsyncJson.h"
#include <ArduinoJson.h>

ApiScreenController::ApiScreenController(AsyncWebServer & _server, ScreenController & screenController)
: _screenController(screenController)
{
        _server.addHandler(new AsyncCallbackJsonWebHandler("/api/move", [this](AsyncWebServerRequest *request, const JsonVariant &json){
        const JsonObject& jsonObj = json.as<JsonObject>();
        auto command = jsonObj.getMember("command").as<String>();
        if(command == "up")
          _screenController.roll(Direction::UP);
        else if (command == "down")
          _screenController.roll(Direction::DOWN);
        else if (command == "stop")
          _screenController.stop();
        else
        {
          request->send(400);
          return;
        }

        request->send(200);
    }));
}