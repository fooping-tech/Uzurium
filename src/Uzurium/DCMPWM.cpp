#include "DCMPWM.h"

//pwm
// PWMに使用するビット数　n=1～16[bit]
const uint8_t nBits_forPWM = 8; 
//PWM周波数の最大値 Maxfreq=80000000.0/2^n[Hz]=312500[Hz]
uint8_t PWM_CH = 0;   // PWMチャンネル
//int PWM_Values = 0; //デューティ　デューティー比 0-256
//MaxDuty=2^n  DutyRatio = Duty/MaxDuty
const double PWM_Frequencies[] = {2000.0, 4000.0, 6000.0, 8000.0, 10000.0, 12000.0, 14000.0,1046,1174,1318,1396,1567,1760,1975,2093};
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
void DCMPWM::changeFreq(int index){
    ledcSetup(PWM_CH, PWM_Frequencies[index], nBits_forPWM);
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
/*
//メイン関数
void DCMPWM::main(){
    //初期化済みの時
    //前回実行時からの経過時間を計算
    uint32_t deltaTime = millis() - cycleTime;
    //経過時間を計算
    uint32_t spentTime = millis() - startTime;
      //所定時間以上経過していたら実行
    if(deltaTime >= TaskSpan){
      if(pattern==0){
        if(spentTime>0)changeFreq(8);
        if(spentTime>100)changeFreq(9);
        if(spentTime>200)changeFreq(10);
        if(spentTime>300)changeFreq(11);
        if(spentTime>400)changeFreq(12);
        if(spentTime>500)changeFreq(8);
        if(spentTime>600)BuzzerOff();
      }
      if(pattern==1){
        if(spentTime>0)changeFreq(8);
        if(spentTime>100)changeFreq(12);
        if(spentTime>200)changeFreq(11);
        if(spentTime>300)changeFreq(10);
        if(spentTime>400)changeFreq(9);
        if(spentTime>500)changeFreq(8);
        if(spentTime>600)BuzzerOff();
      }
      if(pattern==3){
        if(spentTime>0)changeFreq(8);
        if(spentTime>100)changeFreq(9);
        if(spentTime>200)changeFreq(10);
        if(spentTime>300)changeFreq(11);
        if(spentTime>400)changeFreq(12);
        if(spentTime>500)changeFreq(8);
        if(spentTime>600)BuzzerOff();
      }
      //led.counter(BUZZER_counter);
      //counter++;
      //所定時間タスク終了後サイクルタイム初期化
      cycleTime = millis();

    }
  
}*/