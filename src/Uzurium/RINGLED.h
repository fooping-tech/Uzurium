#ifndef RINGLED_H
#define RINGLED_H

//pragma message "No hardware SPI pins defined.  All SPI access will default to bitbanged output" 対策
//#define FASTLED_ESP32_I2S
//#define FASTLED_ALLOW_INTERRUPTS 0
//#define FASTLED_ESP32_I2S true
//#define FASTLED_ALL_PINS_HARDWARE_SPI
//#define FASTLED_ESP32_SPI_BUS HSPI

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