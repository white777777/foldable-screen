#ifndef KEY_CONTROLLER_H
#define KEY_CONTROLLER_H

#include <TaskSchedulerInclude.h>

class KeyController
{
    private:
        Scheduler& _ts;
        bool _isKeyPressed;
        std::function<void(KeyController&, bool)> _callback = [](KeyController& a, bool b){};
        Task _checkTimeoutTask;
        KeyController();
    public:
        KeyController(Scheduler& ts); 
        void PressKey(int timeout);
        void ReleaseKey();
        bool IsKeyPressed();
        void SetCallback(std::function<void(KeyController&, bool)> callback);
};

#endif