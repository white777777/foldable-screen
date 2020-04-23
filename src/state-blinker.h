#pragma once 
#include <TaskSchedulerInclude.h>

class StateBlinker
{
  private:
    Task t;
    bool state = false;
    void Do();
  public:
  StateBlinker(Scheduler & ts);

  void SetDelay(int delay);
  void TurnOff(); 
};