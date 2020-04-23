#ifndef CONFIG_PROVIDER_H
#define CONFIG_PROVIDER_H
#include <EEPROM.h>
#include <array>
// #include <IonDB.h>

enum CONFIG_NODES{
    CONFIG_VERSION = 0,
    PULL_UP_TIME = 1,
    PULL_DOWN_TIME = 2,
    FTP_ENABLED = 3,
    OTA_ENABLED = 4,
    SCREEN_POSITION = 5
};

class ConfigProvider
{
    private:
        std::array<uint8_t, 10> _cache;
    public:
        ConfigProvider();

        ConfigProvider(const ConfigProvider&) = delete;
        ConfigProvider& operator=(const ConfigProvider&) = delete;
        ConfigProvider(ConfigProvider&&) = delete;
        ConfigProvider& operator=(ConfigProvider&&) = delete;

        uint8_t Get(CONFIG_NODES node) const;
        void Set(CONFIG_NODES node, const uint8_t& value);
};

//     private:
//         Dictionary < int, int > _dict;
//         //MasterTable _masterTable;
//     public:
//      ConfigProvider()
//      : _dict(FlatFile < int, int > (0, key_type_numeric_signed, sizeof(int), sizeof(int), 7))
//     {
//         //_dict.
//         //_masterTable.openDictionary(&_dict, 0);
//         //EEPROM.begin(SPI_FLASH_SEC_SIZE);
//     }

//     ~ConfigProvider()
//     {
//         _dict.close();
//         //_masterTable.closeMasterTable();
//         //_masterTable.closeAllMasterTable();
//     }

#endif