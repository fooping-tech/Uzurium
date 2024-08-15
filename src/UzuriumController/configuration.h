#ifndef CONFIGURATION_H
#define CONFIGURATION_H

//Debug
#define ARDUINOTRACE_ENABLE 1 // 0:Disable all traces(TRACE,DUMP)//include ArduinoTrace.hより上に書くこと
#include <ArduinoTrace.h>

#include <M5Unified.h>
#include <esp_now.h>
#include <WiFi.h>
#include "Button.h"
#include "NEOPIXELLED.h"

//Mic
#define ANALOG_PIN 35
//VR
#define ANALOG_VR_PIN 36

#define LED_PIN 19

#define BTN1_PIN 14
#define BTN2_PIN 27
#define BTN3_PIN 13

#endif