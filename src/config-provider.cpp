#include <config-provider.h>
#include <ArduinoLog.h>

ConfigProvider::ConfigProvider()
{
    constexpr int cureentVersion = 13;
    EEPROM.begin(512);
    EEPROM.get(CONFIG_NODES::CONFIG_VERSION, _cache[0]);

    Log.notice("config version: %d\n", _cache[0]);
    if(_cache[0]>cureentVersion)
        _cache[0] = 0;
    if(_cache[0] < 11)
    {
        Set(CONFIG_NODES::FTP_ENABLED, 1);
        Set(CONFIG_NODES::OTA_ENABLED, 1);
    }
    if(_cache[0] < 12)
    {
        Set(CONFIG_NODES::SCREEN_POSITION, 0);
    }
    if(_cache[0] < 13)
    {
        Set(CONFIG_NODES::PULL_UP_TIME, 215);
        Set(CONFIG_NODES::PULL_DOWN_TIME, 200);
    }
    Set(CONFIG_NODES::CONFIG_VERSION, cureentVersion);
    //load
    for(std::size_t i =0; i<_cache.size(); ++i)
        EEPROM.get(i,_cache[i]);
}

uint8_t ConfigProvider::Get(CONFIG_NODES node) const
{
    return _cache[node];
}

void ConfigProvider::Set(CONFIG_NODES node, const uint8_t &value)
{
    if(_cache[node]!= value)
    {
        EEPROM.put(node, value);
        EEPROM.commit();
    }
    _cache[node] = value;
}

IPAddress ConfigProvider::GetMqttHost()
{
    return IPAddress(192,168,2,1);
}

int ConfigProvider::GetMqttPort()
{
    return 1883;
}
//TODO: add NTP config

constexpr int NTP_TIMEOUT = 5000;
constexpr int SHOW_TIME_PERIOD = 1500;
const PROGMEM char *ntpServer = "pool.ntp.org";
constexpr int8_t timeZone = 3;
constexpr int8_t minutesTimeZone = 0;


String ConfigProvider::GetSsid()
{
    return "***";
}

String ConfigProvider::GetPassword()
{
    return "***";
}

String ConfigProvider::GetBlynkAuth()
{
    return "***";
}