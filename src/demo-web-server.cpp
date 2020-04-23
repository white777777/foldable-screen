#include <demo-web-server.h>
#include <ArduinoLog.h>
#include "AsyncJson.h"
#include "ArduinoJson.h"


DemoWebServer::DemoWebServer(ScreenController & screenController, FS & fs, ConfigProvider & configProvider)
: _apiScreenController(_server, screenController)
, _apiConfigController(_server, configProvider)
, _apiLegacyController(_server, screenController)
, _apiStatusController(_server)
, _apiFilesController(_server, fs)
{
    _server.begin();
}
