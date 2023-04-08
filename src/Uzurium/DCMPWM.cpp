#include "DCMPWM.h"

//pwm
// PWMに使用するビット数　n=1～16[bit]
const uint8_t nBits_forPWM = 8; 
//PWM周波数の最大値 Maxfreq=80000000.0/2^n[Hz]=312500[Hz]
uint8_t PWM_CH = 0;   // PWMチャンネル
//int PWM_Values = 0; //デューティ　デューティー比 0-256
//MaxDuty=2^n  DutyRatio = Duty/MaxDuty
const double PWM_Frequencies[] = {2000.0, 4000.0, 6000.0, 8000.0, 10000.0, 12000.0, 14000.0};
// PWM周波数 Maxfreq=80000000.0/2^n[Hz]
const int nPWM_Frequencies = sizeof(PWM_Frequencies)/sizeof(double);

//int on_duty =0;

DCMPWM::DCMPWM(){

}
void DCMPWM::setup(int CH,int PIN){
  if(!_isSetuped){
    pinMode(PIN, OUTPUT);
    PWM_CH = CH;
    //pwm
  // チャンネルと周波数の分解能を設定
  int freqIndex = 1;
  ledcSetup(PWM_CH, PWM_Frequencies[freqIndex], nBits_forPWM);
  // PWM出力ピンとチャンネルの設定
  ledcAttachPin(PIN, PWM_CH);
    _isSetuped =true;
    Serial.println("MOTOR setup was completed.");
  }
}
void DCMPWM::move(int duty){
    //Serial.println("MOTOR move");
  //on_duty =256;
  int ON_DUTY=duty;
  ledcWrite(PWM_CH, ON_DUTY);
}
void DCMPWM::stop(){
  //Serial.println("MOTOR stop");
  int OFF_DUTY= 0;
  ledcWrite(PWM_CH, OFF_DUTY);
}