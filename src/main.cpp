#include <Arduino.h>
#include <foldable-screen.h>
#include <TaskSchedulerDeclarations.h>
#include <TaskScheduler.h>
#include <wifi-controller.h>
#include <demo-web-server.h>
#include <ArduinoLog.h>
#include <FS.h>
#include <ota-controller.h>
#include <state-blinker.h>
#include <TimeLib.h>
#include <NtpClientLib.h>
#ifdef ENABLE_MQTT
#include <mqtt-controller.h>
#endif
#include <blynk-controller.h>
#include <ftp-controller.h>
#include <delays.h>

//service
static ConfigProvider configProvider;
static Scheduler ts;
static StateBlinker blinker(ts);
static WifiController wifiController(blinker, configProvider);
static FS fileSystem(SPIFFS);
static OtaController otaController;
static FtpController ftpController(ts, configProvider);

//MVC Model
static ScreenController screenController(ts, configProvider, blinker);

//MVC controller
static DemoWebServer demoWebServer(screenController, fileSystem, configProvider);
BlynkController blynkController(screenController, configProvider, ts);
#ifdef ENABLE_MQTT
static MqttController mqttController(ts,  configProvider);
#endif
//static FoldableScreen foldableScreen(ts, fileSystem, configProvider, blinker, screenController, demoWebServer);

enum STATE {NORMAL, UPDATING};
STATE state = STATE::NORMAL;

void processSyncEvent (NTPSyncEvent_t ntpEvent) {
    if (ntpEvent < 0) {
        Log.warning ("Time Sync error: %d\n", ntpEvent);
        if (ntpEvent == noResponse)
            Log.warning ("NTP server not reachable\n");
        else if (ntpEvent == invalidAddress)
            Log.warning ("Invalid NTP server address\n");
        else if (ntpEvent == errorSending)
            Log.warning ("Error sending request\n");
        else if (ntpEvent == responseError)
            Log.warning ("NTP response error\n");
    } else {
        if (ntpEvent == timeSyncd) {
            Log.notice ("Got NTP time: %s\n", NTP.getTimeDateString (NTP.getLastNTPSync ()).c_str());
        }
    }
}

static Task ntpTask(TASK_IMMEDIATE, TASK_ONCE, __null, &ts, false);


void loopMethod()
{
  if(configProvider.Get(CONFIG_NODES::OTA_ENABLED))
    otaController.Loop();
}
static Task loopTask(MAIN_DELAY, TASK_FOREVER, &loopMethod, &ts);


void setup() {
  Serial.begin(115200);
  Serial.print("\n[Main] Setup start \n");
  Log.begin(LOG_LEVEL_VERBOSE, &Serial);
  fileSystem.begin();

  wifiController.Init();
  wifiController.WaitForConnection();
  
  NTP.onNTPSyncEvent ([](const NTPSyncEvent_t event) {
    ntpTask.setCallback([event](){processSyncEvent(event);});
    ntpTask.restart();
  });
  //OnWifiConnect
  NTP.begin (DEFAULT_NTP_SERVER, 3, true, 0);

  otaController.OnUpdateStart([](){
    Log.notice(F("[Ota] Start update" CR));
    state = UPDATING;
    fileSystem.end();
  });
  //OnWifiConnect
  otaController.Begin();
  Log.notice(F("[Main] Setup finished" CR));
  loopTask.enable();
}

void loop() {
  ts.execute();
}