#pragma once
#include <AsyncMqttClient.h>
#include <config-provider.h>
#include <TaskSchedulerDeclarations.h>

class MqttController
{
    private:
        Scheduler & _ts;
        AsyncMqttClient _mqttClient;
        ConfigProvider & _configProvider; 
        WiFiEventHandler wifiConnectHandler;
        WiFiEventHandler wifiDisconnectHandler;
        Task MqttReconnectTask;

    public:
        MqttController(Scheduler & ts, ConfigProvider & configProvider);

        void onGotIp(const WiFiEventStationModeGotIP& event);
        void onWifiDisconnect(const WiFiEventStationModeDisconnected& event);

        void connectToMqtt();
        void onMqttConnect(bool sessionPresent);
        void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
        void onMqttSubscribe(uint16_t packetId, uint8_t qos);
        void onMqttUnsubscribe(uint16_t packetId);
};