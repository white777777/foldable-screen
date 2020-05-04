#include <mqtt-controller.h>
#include <ArduinoLog.h>

using namespace std::placeholders;

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  Log.notice("[Mqtt] Publish received.");
  Log.notice(F("  topic: %s\n" ), topic);
  Log.notice("  qos: %d\n", properties.qos);
  Log.notice("  dup: %T\n", properties.dup);
  Log.notice("  retain: %T\n", properties.retain);
  Log.notice("  len: %d\n", len);
  Log.notice("  index: %d\n", index);
  Log.notice("  total: %d\n", total);
}

void onMqttPublish(uint16_t packetId) {
  Log.notice(F("[Mqtt] Publish acknowledged." CR));
  Log.notice("  packetId: %d\n", packetId);
}

MqttController::MqttController(Scheduler &ts, ConfigProvider & configProvider)
:_ts(ts)
,_configProvider(configProvider)
,MqttReconnectTask(5000, TASK_FOREVER, std::bind(&MqttController::connectToMqtt, this), &ts)
{
    _mqttClient
    .onConnect(std::bind(&MqttController::onMqttConnect, this, _1))
    .onDisconnect(std::bind(&MqttController::onMqttDisconnect, this, _1))
    .onSubscribe(std::bind(&MqttController::onMqttSubscribe, this, _1, _2))
    .onUnsubscribe(std::bind(&MqttController::onMqttUnsubscribe, this, _1))
    .onMessage(onMqttMessage)
    .onPublish(onMqttPublish)
    .setServer(_configProvider.GetMqttHost(), _configProvider.GetMqttPort());

    wifiConnectHandler = WiFi.onStationModeGotIP(std::bind(&MqttController::onGotIp, this, _1));
    wifiDisconnectHandler = WiFi.onStationModeDisconnected(std::bind(&MqttController::onWifiDisconnect, this, _1));

    // if(WiFi.isConnected())
    //     connectToMqtt();

}

void MqttController::onGotIp(const WiFiEventStationModeGotIP& event) 
{
  MqttReconnectTask.restart();
}

void MqttController::onWifiDisconnect(const WiFiEventStationModeDisconnected& event)
{
  MqttReconnectTask.disable();// ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
}

void MqttController::connectToMqtt() {
  if(MqttReconnectTask.isFirstIteration())
  return;
  Log.notice(F("[Mqtt] Connecting to MQTT..." CR));
  _mqttClient.connect();
}

void MqttController::onMqttConnect(bool sessionPresent) {
  Log.notice(F("[Mqtt] Connected to MQTT." CR));
  Log.notice("[Mqtt] Session present: %T\n", sessionPresent);

  uint16_t packetIdSub = _mqttClient.subscribe("test/lol", 2);
  Log.notice("[Mqtt] Subscribing at QoS 2, packetId: %d\n", packetIdSub);
  _mqttClient.publish("test/lol", 0, true, "test 1");
  Log.notice("[Mqtt] Publishing at QoS 0\n");
  uint16_t packetIdPub1 = _mqttClient.publish("test/lol", 1, true, "test 2");
  Log.notice("[Mqtt] Publishing at QoS 1, packetId: %d\n", packetIdPub1);
  uint16_t packetIdPub2 = _mqttClient.publish("test/lol", 2, true, "test 3");
  Log.notice("[Mqtt] Publishing at QoS 2, packetId: %d\n", packetIdPub2);
}

void MqttController::onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Log.notice(F("[Mqtt] Disconnected from MQTT." CR));

  if (WiFi.isConnected()) {
    MqttReconnectTask.restart();
  }
}

void MqttController::onMqttSubscribe(uint16_t packetId, uint8_t qos) {
  Log.notice(F("[Mqtt] Subscribe acknowledged." CR));
  Log.notice("  packetId: %d", packetId);
  Log.notice(CR);
  Log.notice("  qos: %d", qos);
  Log.notice(CR);
}

void MqttController::onMqttUnsubscribe(uint16_t packetId) {
  Log.notice(F("[Mqtt] Unsubscribe acknowledged." CR));
  Log.notice("  packetId: %d", packetId);
  Log.notice(CR);
}

