#include <Arduino.h>
#include <foldable-screen.h>
#include <TaskSchedulerInclude.h>
#include <TaskScheduler.h>
#include <wifi-controller.h>
#include <demo-web-server.h>
#include <ArduinoLog.h>
#include <FS.h>
#include <ESP8266FtpServer.h>  
#include <ota-controller.h>
#include <state-blinker.h>


Scheduler ts;
StateBlinker blinker(ts);
WifiController wifiController(blinker);
FS fileSystem(SPIFFS);
ConfigProvider configProvider;
FoldableScreen foldableScreen(ts, fileSystem, configProvider, blinker);
FtpServer ftpSrv;
OtaController otaController;

enum STATE {NORMAL, UPDATING};
STATE state = STATE::NORMAL;

void setup() {
  Serial.begin(115200);
  Log.begin(LOG_LEVEL_VERBOSE, &Serial);
  fileSystem.begin();
  wifiController.Init();
  wifiController.WaitForConnection();
  ftpSrv.begin("admin", "admin");
  Log.notice(F("Setup finished" CR));

  otaController.OnUpdateStart([](){
    Log.notice(F("Start update" CR));
    state = UPDATING;
    fileSystem.end();
  });
  otaController.Begin();
}

void loop() {
  if(state == STATE::NORMAL)
  {
    //TODO: async
    if(configProvider.Get(CONFIG_NODES::FTP_ENABLED))
      ftpSrv.handleFTP(); 
    ts.execute();
  }
  if(configProvider.Get(CONFIG_NODES::OTA_ENABLED))
    otaController.Loop();
}