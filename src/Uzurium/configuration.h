#ifndef CONFIGURATION_H
#define CONFIGURATION_H

//Debug
#define ARDUINOTRACE_ENABLE 1  // 0:Disable all traces
#include <ArduinoTrace.h>

#include <M5Unified.h>
#include "DCMPWM.h"
#include "RINGLED.h"
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
#define NUMBER 5

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
//PID定数
float Kp = 0.001;
const float Ki = 0.000001;
const float Kd = 1;

#endif