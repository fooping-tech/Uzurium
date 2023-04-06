
volatile unsigned long lastPulseTime=1;
volatile unsigned long pulseInterval=100000000;//最大値に設定
volatile uint16_t rpm=0; // RPMの変数 uint16_t:65535rpmまで計測可能

float nowRPM =0;
float beforeRPM=0;
float diffRPM=0;
float TargetRPM = 0;
float outRPM=5000;//脱調RPM
float inRPM=0;//回転開始RPM

int outDuty=0;//脱調DUTY
int inDuty=0;//回転開始DUTY
bool outFlag =false;//脱調フラグ
bool inFlag =false;

void PHOTO_setup(){
  pinMode(PHOTO_PIN,INPUT);
  attachInterrupt(digitalPinToInterrupt(PHOTO_PIN), rpm_fun, FALLING);
  lastmillis = millis(); // タイムアウトのための時間変数を初期化

}

void PHOTO_Set_TargetRPM(float target){
  TargetRPM = target;
}
float PHOTO_Check_TargetRPM(){
  return TargetRPM;
}


float PHOTO_Check_rpm(){
  return rpm;
}

float PHOTO_Check_nowRPM(){
  return nowRPM;
}

float PHOTO_Check_outRPM(){
  return outRPM;
}
float PHOTO_Check_diffRPM(){
  diffRPM = nowRPM - beforeRPM;
  beforeRPM = nowRPM;
  return diffRPM;
}

//AverageRPM
#define STOCK_RPMS 15
int stock_rpm[STOCK_RPMS];
int stock_rpm_num=0;
void PHOTO_CalcNowRPM(){
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

void rpm_fun() {  // タコメーターのパルスが検出されたときに実行する関数
  unsigned long cur = micros();
  unsigned long dif = cur - lastPulseTime; // 前回のエッジとの差分
  pulseInterval = (pulseInterval - (pulseInterval >> 2)) + (dif >> 2); // 滑らかに
  lastPulseTime = cur;
  //pmcount++;
}
//タイムアウトしたらrpm初期化
bool PHOTO_CheckTimeout(){
  if(micros() - lastPulseTime > 100000){
    rpm = 0;
    pulseInterval=100000000;
    return true;
  }else{
    return false;
  }
}

//脱調判定
void PHOTO_check(){
  //rpm start check(RPM変化量10以上、直前RPM 100以下、)
    if(diffRPM>10 && beforeRPM<100 && !inFlag){
      inFlag=true;
      inDuty=SPEED_CheckDuty();
      inRPM = nowRPM;
    }
    //rpm out check(diffRPM>200を脱調と判定)
    if(diffRPM>200 && !outFlag){
      outFlag=true;//脱調フラグ立てる
      outDuty = SPEED_CheckDuty();//脱調時のDUTYを記録
      outRPM = nowRPM;
    }
    //reset
    if(CheckSQ() && outFlag){
      StopDuty();
      if(nowRPM<100){
        StartDuty(0);
        outFlag=false;
      }
    }
}