#pragma once
#include <ESPAsyncWebServer.h>

class ApiFilesController
{
    private:
        FS& _fs;

        bool handleFileRead(AsyncWebServerRequest* request);

    public:
    ApiFilesController(AsyncWebServer & server, FS& fs)
    :_fs(fs)
    {

    server.serveStatic("/", _fs, "/")
    .setDefaultFile("index.html");

    server.onNotFound([this](AsyncWebServerRequest* request){
        // if(!handleFileRead(request))
            request->send(404, "text/plain", "Not Found");
    });

    }
};

