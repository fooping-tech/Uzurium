#ifndef BUZZER_H
#define BUZZER_H

#include <M5Unified.h>
#include "DCMPWM.h"
#include "RINGLED.h"

class BUZZER{
  public:
    BUZZER(){//コンストラクタ
      TaskSpan=10;
      startTime = millis();
      cycleTime = millis();
      setup(0);
    }
    void finish(){
        if(pattern==0 || pattern==1)motor.move(0);
    }
    uint32_t CheckSpentTime();
    void setup(int pat){
      pattern = pat;
      if(pattern==0 || pattern==1)motor.move(20);
    }
    void mainloop();
    void main();
  private:
    int TaskSpan;
    uint32_t startTime;
    uint32_t cycleTime;
    uint32_t deltaTime;
    uint32_t spentTime;
    int pattern;
    int counter;
    DCMPWM motor = DCMPWM();
    RINGLED led = RINGLED();
};
#endif