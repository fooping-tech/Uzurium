#ifndef CONFIGURATION_H
#define CONFIGURATION_H

//Debug
#define ARDUINOTRACE_ENABLE 1  // 0:Disable all traces(TRACE,DUMP)
#include <ArduinoTrace.h>

#include <M5Unified.h>
#include "DCMPWM.h"
#include "RINGLED.h"
#include "PHOTO.h"
#include "MODE.h"

#include <esp_now.h>
#include <WiFi.h>

//INA219
#include <Wire.h>
#include <Adafruit_INA219.h>
Adafruit_INA219 ina219;


//motor pin setting
#define MOTOR_PIN 23 //ATOM:32
//#define MOTOR_PIN 33 //STAMP:33

//UzuriumNumber
//#define NUMBER 1

//PWM CH(for MotorDrive)
#define CHANNEL 2

#define LED_PIN 22 // M5Atom:26
//#define LED_PIN 32 //M5Stamp:32

//PhotoSensor
#define PHOTO_PIN 19

//Mic
#define ANALOG_PIN 33

//SW
#define SW_PIN 25
//SW
#define TEST_SW_PIN 21

//PID定数
float Kp = 0.0007;
const float Ki = 0;
const float Kd = 0;
//LED Brightness
int LedBrightness =75;
int LedBrightnessLow =10;

#endif