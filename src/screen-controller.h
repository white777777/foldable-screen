#ifndef SCREEN_CONTROLLER_H
#define SCREEN_CONTROLLER_H

#include <TaskSchedulerDeclarations.h>
#include <config-provider.h>
#include <state-blinker.h>
#include <pin-config.h>
#include <list>

enum Direction {UP, DOWN};
enum MOVE_STATE {INVALID = 0, MOVE_UP=1, MOVE_STOP=2, MOVE_DOWN=3};

typedef std::shared_ptr<std::function<void()>> ScreenEventHandler;
constexpr uint8_t undefinedPin = 255;

class ScreenController{
    private:
        const uint8_t pinUp = PIN_UP;
        const uint8_t pinDown = PIN_DOWN;
        uint8_t _currentPin = undefinedPin;

        unsigned long _startTime =0;
        unsigned long _fullMoveTime = 0;//movement time

        std::list<ScreenEventHandler> sCbEventList;

        Task stopRollTask;
        Task startRollTask;
        Task notifyTask;
        ConfigProvider & _config;
        StateBlinker & _blinker;

        void roll(uint8_t pin, unsigned long time);
        uint8_t DirectionToPin(Direction direction);
        unsigned long DirectionToTime(Direction direction);
        void stop2();
        uint8_t calcPosition();
    public:
        ScreenController(Scheduler & ts, ConfigProvider& config, StateBlinker & blinker);
        
        void roll(Direction direction);        
        void roll(Direction direction, unsigned long time);
        void startRoll(Direction direction);
        void stop();

        MOVE_STATE getMoveState();
        uint8_t getPosition();

        ScreenEventHandler onStateChanged(std::function<void()> f);
        void _eventCallback();

        //TODO: get state
        //TODO: state changed event
};
#endif