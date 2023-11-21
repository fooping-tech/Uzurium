#ifndef RINGLED_H
#define RINGLED_H

#include <M5Unified.h>
#include <FastLED.h>
//LED Setting
#define FPS 120
#define NUM_LEDS 18


class RINGLED{
  public:
    RINGLED();
    void setup(int);
    void flash(int);//color
    void clyon();
    void fire();
    void fire2(int,int);
    void fade();
    void inspection(int);
    void stop();
    void pacifica();
    void setbrightness(int);//brightness
    void counter(int);
    void level(int);

  private:
    bool _isSetuped;
};
#endif