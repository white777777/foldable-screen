#pragma once
#include <ESP8266WiFi.h>
#include <state-blinker.h>
#include <config-provider.h>

class WifiController
{
    private:
        StateBlinker & _blinker;
        WiFiEventHandler _onStationModeConnected;
        WiFiEventHandler _onStationModeDisconnected;
        WiFiEventHandler _onStationModeGotIP;
        ConfigProvider & _configProvider;
         
    public:
        WifiController(StateBlinker & blinker, ConfigProvider & configProvider);
        void Init();
        void WaitForConnection();
};
