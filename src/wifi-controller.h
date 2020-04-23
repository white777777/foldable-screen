#pragma once
#include <ESP8266WiFi.h>
#include <state-blinker.h>

class WifiController
{
    private:
        StateBlinker & _blinker;
        WiFiEventHandler _onStationModeConnected;
        WiFiEventHandler _onStationModeDisconnected;
        WiFiEventHandler _onStationModeGotIP;
         
    public:
        WifiController(StateBlinker & blinker);
        void Init();
        void WaitForConnection();
};
