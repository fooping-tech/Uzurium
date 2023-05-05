#ifndef CONFIGURATION_H
#define CONFIGURATION_H

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
#define MOTOR_PIN 32 //ATOM:32
//#define MOTOR_PIN 33 //STAMP:33

//UzuriumNumber
#define NUMBER 5

//PWM CH(for MotorDrive)
#define CHANNEL 2

#define LED_PIN 26 // M5Atom:26
//#define LED_PIN 32 //M5Stamp:32

//PhotoSensor
#define PHOTO_PIN 19

//Mic
#define ANALOG_PIN 33
//PID定数
const float Kp = 0.0005;
const float Ki = 0.000001;
const float Kd = 0.01;

#endif