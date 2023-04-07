#ifndef DCMPWM_H
#define DCMPWM_H

#include <M5Unified.h>

class DCMPWM{
  public:
    DCMPWM();
    void setup(int,int);//pwm ch , pin
    void move(int);//duty
    void stop();
  private:
    bool _isSetuped;
};
#endif