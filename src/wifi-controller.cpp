#include <wifi-controller.h>
#include <ESP8266WiFi.h>
#include <ArduinoLog.h>

WifiController::WifiController(StateBlinker & stateBlinker)
: _blinker(stateBlinker)
{
        // WiFi.config(IPAddress(192, 168, 2, 201), 
    // IPAddress(192,168,2,1),
    // IPAddress(255,255,255,0),
    // IPAddress(192,168,2,1),
    // IPAddress(8,8,8,8)
    // );
    
    //TODO: remove subscribe in destructor
    _onStationModeConnected = WiFi.onStationModeConnected([this](const WiFiEventStationModeConnected & e){
        Log.notice(F("Connected" CR));
        _blinker.SetDelay(2000*TASK_MILLISECOND);
    });
    _onStationModeDisconnected = WiFi.onStationModeDisconnected([this](const WiFiEventStationModeDisconnected & e){
        Log.notice(F("Disconnected" CR));
        _blinker.SetDelay(50*TASK_MILLISECOND);
    });
    _onStationModeGotIP = WiFi.onStationModeGotIP([this](const WiFiEventStationModeGotIP & e){
        Log.notice(F("Received ip: %s" CR), e.ip.toString().c_str());
        _blinker.TurnOff();
    });
}

void WifiController::Init()
{
    Log.notice(F("Init wifi controller" CR));

    WiFi.begin("Keenetic", "***");
}

void WifiController::WaitForConnection()
{
    //blocking
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Log.notice(F("Connection Failed!" CR));
        delay(5000);
    }
}