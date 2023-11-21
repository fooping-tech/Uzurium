#include "PHOTO.h"

volatile int PHOTO::counter=0;
volatile unsigned long PHOTO::lastPulseTime=1;
volatile unsigned long PHOTO::pulseInterval=100000000;//最大値に設定
volatile uint16_t PHOTO::rpm=0;
int PHOTO::photo_pin=19;

PHOTO::PHOTO()
  : _OffsetDutyInitValue(0)
  , _LimitRPMD(800)
  , _StartRPMD(10)
  , _LowRPM(100)
  , _MAX_DUTY(70) //0-256
  , _MIN_DUTY(0)
  , _M_DUTY(0)
{
    //photo_pin=0;
    outRPM =6000 ;//脱調RPM
    inRPM=0;//回転開始RPM
    outDuty=0;//脱調DUTY=0
    inDuty=0;//回転開始DUTY=0

    //float P,I,D,preP;
    duty_p=0;//=0
    duty=0;//=0

    //lastPulseTime=1;
    //pulseInterval=100000000;//最大値に設定
    //rpm =0 ; // RPMの変数 uint16_t:65535rpmまで計測可能 =0

    nowRPM=0;// =0
    beforeRPM=0;//=0
    diffRPM=0;//=0
    TargetRPM=0;// = 0
    offset=0;//オフセットduty=0

    outFlag=false;//脱調フラグ =false
    inFlag=false;//回転開始フラグ =false
    stock_rpm_num=0;

    //setup(photo_pin);
}

void PHOTO::setup(int pin){
  photo_pin = pin;
  pinMode(pin,INPUT);
  SetInterrupt();
  //lastmillis = millis(); // タイムアウトのための時間変数を初期化
  SetOffsetDuty(_OffsetDutyInitValue);//offsetを初期値にセット
  Serial.println("PHOTO setup was completed.");
}

void PHOTO::reset(){
//RPM平均算出用ストック初期化
  DeleteNowRPMStock();
  //RPM初期化
  rpm=0;
  nowRPM = 0;
  beforeRPM = 0;
  //
  pulseInterval=100000000;
  lastPulseTime=1;
  //ターゲット回転数初期化
  SetTargetRPM(0);

  P=0;
  I=0;
  D=0;
  duty_p=0;
  duty=0;
}
void PHOTO::StopInterrupt(){
  detachInterrupt(digitalPinToInterrupt(photo_pin)); 
}
void PHOTO::SetInterrupt(){
 attachInterrupt(digitalPinToInterrupt(photo_pin), PHOTO::Measure, FALLING);
}
void PHOTO::Measure(){
  //StopInterrupt();
  unsigned long cur = micros();
  unsigned long dif = cur - lastPulseTime; // 前回のエッジとの差分
  pulseInterval = (pulseInterval - (pulseInterval >> 2)) + (dif >> 2); // 滑らかに
  lastPulseTime = cur;
  rpm = 60000000 / (pulseInterval);
  //pmcount++;
  //MODE_C_CountUp();
  counter++;
//  Serial.println(counter);
  //SetInterrupt();
}
int PHOTO::CheckCounter(){
  return counter;
}
void PHOTO::CalcRPM(){
  rpm = 60000000 / (pulseInterval);
}
void PHOTO::SetTargetRPM(float target){
TargetRPM = target;
}
float PHOTO::CheckTargetRPM(){
return TargetRPM;
}
float PHOTO::CheckRpm(){
return rpm;
}
float PHOTO::CheckNowRPM(){
return nowRPM;
}
float PHOTO::CheckOutRPM(float margin){
return outRPM + margin;
}
float PHOTO::CheckInRPM(float margin){
return inRPM + margin;
}
float PHOTO::CheckDiffRPM(){
  diffRPM = nowRPM - beforeRPM;
  beforeRPM = nowRPM;
  return diffRPM;
}
void PHOTO::CalcNowRPM(){
stock_rpm_num++;
  if(stock_rpm_num >=STOCK_RPMS){
    stock_rpm_num=0;
  }
  stock_rpm[stock_rpm_num]=rpm;
  float result=0;

  for(int i = 0;i < STOCK_RPMS;i++){
    result +=stock_rpm[i];
  }
  result = result/(STOCK_RPMS);
  nowRPM = result;
}
void PHOTO::DeleteNowRPMStock(){
  for(int i = 0;i < STOCK_RPMS;i++){
    stock_rpm[i]=0;
  }
}
int PHOTO::CheckMaxDuty(){
  return _MAX_DUTY;
}
int PHOTO::CheckMinDuty(){
  return _MIN_DUTY;
}


bool PHOTO::CheckTimeout(){
  if(micros() - lastPulseTime > 500000){
    Serial.println("Photo_TimeOut!");
    rpm = 0;
    pulseInterval=100000000;
    inFlag=false;
    outFlag=false;
    return true;
  }else{
    return false;
  }
}
void PHOTO::CheckOutOfStep(){
//回転開始(RPM変化量+*以上、直前RPM ***以下)
    if(diffRPM>_StartRPMD && beforeRPM<_LowRPM && !inFlag){
      inFlag=true;//回転開始フラグを立てる
      //offsetが初期値のときは、回転開始DUTYを記録
      if(offset == _OffsetDutyInitValue){
        inDuty=CheckDuty();
        inRPM = nowRPM;//回転開始RPMを記録
      }
    }
    //rpm out check(diffRPM>PHOTO_LimitRPMDを脱調と判定)
    if(diffRPM>_LimitRPMD && !outFlag){
      outFlag=true;//脱調フラグ立てる
      outDuty = CheckDuty();//脱調時のDUTYを記録
      outRPM = nowRPM;//脱調時のRPMを記録
      //DUMP(diffRPM);
      //TRACE();
    }
}
//現在のDutyを返す
int PHOTO::CheckDuty(){
  return duty;
}
//現在のDutyを返す
int PHOTO::CheckDuty_p(){
  return duty_p;
}
bool PHOTO::CheckOutFlag(){
  return outFlag;
}
void PHOTO::SetDuty(int d){
  duty = d;
}
void PHOTO::SetOffsetDuty(int d){
  offset = d;
}
void PHOTO::ClacDuty(uint32_t deltaTime){
     P = TargetRPM - nowRPM;
      I += P * deltaTime;
      D = (P - preP)/deltaTime;
      preP = P;
      //duty += Kp * P + Kd * D + Ki * I;
      //float duty_f += Kp * P;
      duty_p += Kp * P + Kd * D;
      duty = duty_p + offset;

      //上下限すり切り,DUTY制限
      if(duty>=_MAX_DUTY){
        duty=_MAX_DUTY;
        duty_p=duty;
      }
      if(duty<_MIN_DUTY){
        duty=_MIN_DUTY;
        duty_p=duty;
      }
}
void PHOTO::IncreaseDuty(int d){
  duty+=d;
  if(duty>=_MAX_DUTY)duty=_MAX_DUTY;
}
void PHOTO::DecreaseDuty(int d){
  duty-=d;
  if(duty<=_MIN_DUTY)duty=_MIN_DUTY;
}
void PHOTO::SetKp(float k){
  Kp = k;
}