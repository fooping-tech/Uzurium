#ifndef NEOPIXELLED_H
#define NEOPIXELLED_H

#include <M5Unified.h>
//#include <M5Atom.h>

#include <FastLED.h>
//LED Setting
#define FPS 120
#define NUM_LEDS 22


class NEOPIXELLED{
  public:
    NEOPIXELLED();
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
    void level2(int,int);

  private:
    bool _isSetuped;
};
#endif