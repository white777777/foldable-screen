#include <config-provider.h>

ConfigProvider::ConfigProvider()
{
    EEPROM.begin(512);
    EEPROM.get(CONFIG_NODES::CONFIG_VERSION, _cache[0]);
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
    Set(CONFIG_NODES::CONFIG_VERSION, 13);
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

