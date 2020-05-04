#include <wifi-controller.h>
#include <ESP8266WiFi.h>
#include <ArduinoLog.h>

WifiController::WifiController(StateBlinker & stateBlinker, ConfigProvider & configProvider)
: _blinker(stateBlinker)
, _configProvider(configProvider)
{
    WiFi.mode(WiFiMode_t::WIFI_STA);
    WiFi.setPhyMode(WiFiPhyMode_t::WIFI_PHY_MODE_11N);
    WiFi.hostname("foldable-screen");
    _onStationModeConnected = WiFi.onStationModeConnected([this](const WiFiEventStationModeConnected & e){
        Log.notice(F("[Wifi] Connected. ssid:%s,  channel: %d" CR), 
        e.ssid.c_str(), e.channel);
        _blinker.SetDelay(2000*TASK_MILLISECOND);
    });
    _onStationModeDisconnected = WiFi.onStationModeDisconnected([this](const WiFiEventStationModeDisconnected & e){
        Log.notice(F("[Wifi] Disconnected. Reason: %d" CR), e.reason);
        _blinker.SetDelay(50*TASK_MILLISECOND);
        WiFi.reconnect();
    });
    _onStationModeGotIP = WiFi.onStationModeGotIP([this](const WiFiEventStationModeGotIP & e){
        Log.notice(F("[Wifi] Received ip: %s" CR), e.ip.toString().c_str());
        _blinker.TurnOff();
    });
}

void WifiController::Init()
{
    Log.notice(F("[Wifi] Init" CR));

    WiFi.begin(_configProvider.GetSsid(), _configProvider.GetPassword());
}

void WifiController::WaitForConnection()
{
    //blocking
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Log.notice(F("[Wifi] Connection Failed!" CR));
        delay(5000);
    }
}