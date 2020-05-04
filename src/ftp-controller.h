#pragma once
#include <TaskScheduler.h>
#include <ESP8266FtpServer.h>  
#include <config-provider.h>
#include <delays.h>

class FtpController{
    private:
    FtpServer ftpSrv;
    ConfigProvider & configProvider;
    Task ftpTask;
    WiFiEventHandler handler1;
    WiFiEventHandler handler2;

    void loop(){
        //if(state == STATE::NORMAL)
        {
            if(configProvider.Get(CONFIG_NODES::FTP_ENABLED))
                ftpSrv.handleFTP(); 
        }
    }
    public:
        FtpController(Scheduler &ts, ConfigProvider& configProvider)
        : configProvider(configProvider)
        , ftpTask(FTP_DELAY, TASK_FOREVER, std::bind(&FtpController::loop, this), &ts)
        {
            //TODO: on wifi connect;
            ftpSrv.begin("admin", "admin");
            handler1 = WiFi.onStationModeGotIP([this](const WiFiEventStationModeGotIP & ev){
                ftpTask.enable();
            });
            handler2 = WiFi.onStationModeDisconnected([this](const WiFiEventStationModeDisconnected & ev){
                ftpTask.disable();
            });
        }
};