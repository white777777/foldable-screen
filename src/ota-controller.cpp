#include <ota-controller.h>
#include <ArduinoLog.h>

OtaController::OtaController(){
        
    // Port defaults to 8266
    ArduinoOTA.setPort(8266);

    // Hostname defaults to esp8266-[ChipID]
    ArduinoOTA.setHostname("foldable-screen");

    // No authentication by default
    // ArduinoOTA.setPassword("admin");

    // Password can be set with it's md5 value as well
    // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
    // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

    ArduinoOTA.onStart([this]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
        type = "sketch";
        } else { // U_FS
        type = "filesystem";
        }

        // NOTE: if updating FS this would be the place to unmount FS using FS.end()
        Serial.print("Start updating " + type);
        _onUpdateStart();
    });
    ArduinoOTA.onEnd([]() {
        Log.notice("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) {
        Log.warning("Auth Failed");
        } else if (error == OTA_BEGIN_ERROR) {
        Log.warning("Begin Failed");
        } else if (error == OTA_CONNECT_ERROR) {
        Log.warning("Connect Failed");
        } else if (error == OTA_RECEIVE_ERROR) {
        Log.warning("Receive Failed");
        } else if (error == OTA_END_ERROR) {
        Log.warning("End Failed");
        }
    });
}

void OtaController::Begin()
{
    ArduinoOTA.begin();
}

void OtaController::Loop()
{
    ArduinoOTA.handle();
}

void OtaController::OnUpdateStart(std::function<void()> f){
    _onUpdateStart = f;
}