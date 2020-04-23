#ifndef SCREEN_CONTROLLER_H
#define SCREEN_CONTROLLER_H

#include <TaskSchedulerInclude.h>
#include <config-provider.h>
#include <state-blinker.h>

enum Direction {UP, DOWN};
class ScreenController{
    private:
        Scheduler & ts;

        //TODO: config with pins
        uint8_t pinUp = 2;
        uint8_t pinDown = 4 ;
        uint8_t _currentPin = 0;

        unsigned long _startTime =0;
        unsigned long _zzzTime = 0;

        Task stopRollTask;
        Task startRollTask;
        ConfigProvider & _config;
        StateBlinker & _blinker;

        void roll(uint8_t pin, unsigned long time);
        uint8_t DirectionToPin(Direction direction);
        unsigned long DirectionToTime(Direction direction);
        void stop2();
    public:
        ScreenController(Scheduler & ts, ConfigProvider& _config, StateBlinker & _blinker);
        
        void roll(Direction direction);        
        void roll(Direction direction, unsigned long time);
        void startRoll(Direction direction);
        void stop();
};
#endif