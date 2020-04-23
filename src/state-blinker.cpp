  #include <state-blinker.h>
  #include <Arduino.h>

  StateBlinker::StateBlinker(Scheduler & ts)
  : t(1*TASK_SECOND, -1L, [this](){Do();}, &ts, false)
  {
    pinMode(16, OUTPUT);
    digitalWrite(16, HIGH);
    t.enable();
  }

  void StateBlinker::Do()
  {
    state = !state;
    digitalWrite(16, state);
  }

  void StateBlinker::SetDelay(int delay)
  {
    t.setInterval(delay);
    t.restart();
  }

  void StateBlinker::TurnOff()
  {
    t.disable();
    state = false;
    digitalWrite(16, HIGH);
  }