#ifndef CONFIGURATION_H
#define CONFIGURATION_H

//#include <M5Unified.h>
#include <M5Atom.h>
#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>

#include "DCMPWM.h"
#include "RINGLED.h"
#include "PHOTO.h"
#include "MODE.h"
#include "SW.h"

//motor pin setting
#define MOTOR_PIN 23

//motor PWM CH(for MotorDrive)
#define CHANNEL 2

//led pin setting
#define LED_PIN 22

//PhotoSensor pin setting
#define PHOTO_PIN 19

//Analog pin setting
#define ANALOG_PIN 33

//SW setting モーメンタリ
#define SW_PIN 25

//SW setting オルタネート
#define TEST_SW_PIN 21

int Uzurium_Number = 0;

#endif
