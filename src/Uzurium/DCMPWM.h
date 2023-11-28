#ifndef DCMPWM_H
#define DCMPWM_H

//#include <M5Unified.h>
#include <M5Atom.h>



class DCMPWM{
  public:
    DCMPWM();
    void setup(int,int);//pwm ch , pin
    void move(int);//duty
    void stop();
    void changeFreq(int);
    void BuzzerOn(int pat){
      TaskSpan=10;
      startTime = millis();
      cycleTime = millis();
      pattern = pat;
      if(pattern==0 || pattern==1)move(20);
    }
    void BuzzerOff(){
      if(pattern==0 || pattern==1)move(0);
      pattern=0;
    }
  private:
    bool _isSetuped;
    int TaskSpan;
    uint32_t startTime;
    uint32_t cycleTime;
    uint32_t deltaTime;
    uint32_t spentTime;
    int pattern=0;
    int counter=0;
};
#endif