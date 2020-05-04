#pragma once
#include <screen-controller.h>
#include <config-provider.h>
#include <TaskSchedulerDeclarations.h>

class BlynkController
{
    ScreenController & _screenController;
    Task runTask;
    WiFiEventHandler handler1;
    WiFiEventHandler handler2;
    ScreenEventHandler handler3;
    String auth;
    bool isInitiated = false;
    public:
        BlynkController(ScreenController & ScreenController, ConfigProvider & configProvider,
        Scheduler & ts);
        void In2(int value);
        void run();
        void UpdateAppState(); 
};
