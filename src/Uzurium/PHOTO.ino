const int PHOTO_OffsetDutyInitValue = 100;//初期値DUTY
const int PHOTO_LimitRPMD = 800; //脱調判定回転数変化
const int PHOTO_StartRPMD = 10;//回転開始回転数変化
const int PHOTO_LowRPM = 100; //低回転判定閾値

const int MAX_DUTY =200; //0~256
const int MIN_DUTY =80; //0~256

const int M_DUTY = 0; //回転開始DUTYマージン

float outRPM=6000;//脱調RPM
float inRPM=0;//回転開始RPM
int outDuty=0;//脱調DUTY
int inDuty=0;//回転開始DUTY


//PID
float P,I,D,preP;
float duty_p=0;
int duty=0;

volatile unsigned long lastPulseTime=1;
volatile unsigned long pulseInterval=100000000;//最大値に設定
volatile uint16_t rpm=0; // RPMの変数 uint16_t:65535rpmまで計測可能

float nowRPM =0;
float beforeRPM=0;
float diffRPM=0;
float TargetRPM = 0;
int offset=50;//オフセットduty

bool outFlag =false;//脱調フラグ
bool inFlag =false;//回転開始フラグ

void PHOTO_setup(){
  pinMode(PHOTO_PIN,INPUT);
  PHOTO_SetInterrupt();
  lastmillis = millis(); // タイムアウトのための時間変数を初期化
  PHOTO_SetOffsetDuty(PHOTO_OffsetDutyInitValue);//offsetを初期値にセット
  Serial.println("PHOTO setup was completed.");
}
void PHOTO_Reset(){
  //RPM平均算出用ストック初期化
  PHOTO_DeleteNowRPMStock();
  //RPM初期化
  rpm=0;
  nowRPM = 0;
  beforeRPM = 0;
  //
  pulseInterval=100000000;
  lastPulseTime=1;
  //ターゲット回転数初期化
  PHOTO_SetTargetRPM(0);
  //回転開始DUTYを初期DUTYに設定
  if(inDuty-M_DUTY>0)PHOTO_SetOffsetDuty(inDuty - M_DUTY);
  //P項を変更
  //Kp = 0.5;

  DUMP(inRPM);
  DUMP(outRPM);
  DUMP(inDuty);
  DUMP(outDuty);

  P=0;
  I=0;
  D=0;
  duty_p=0;
}
// 割り込み解除
void PHOTO_StopInterrupt(){
  detachInterrupt(digitalPinToInterrupt(PHOTO_PIN)); 
}
// 割り込み設定
void PHOTO_SetInterrupt(){
  attachInterrupt(digitalPinToInterrupt(PHOTO_PIN), PHOTO_Measure, FALLING);
}
void PHOTO_CalcRPM(){
  rpm = 60000000 / (pulseInterval);
}
void PHOTO_SetTargetRPM(float target){
  TargetRPM = target;
}
float PHOTO_CheckTargetRPM(){
  return TargetRPM;
}


float PHOTO_CheckRpm(){
  return rpm;
}

float PHOTO_CheckNowRPM(){
  return nowRPM;
}
//脱調RPMを返す
float PHOTO_CheckOutRPM(float margin){
  return outRPM + margin;
}
float PHOTO_CheckInRPM(float margin){
  return inRPM + margin;
}

float PHOTO_CheckDiffRPM(){
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

//RPM_Reset
void PHOTO_DeleteNowRPMStock(){
  for(int i = 0;i < STOCK_RPMS;i++){
    stock_rpm[i]=0;
  }
}

// パルスが検出されたときに実行する関数
void PHOTO_Measure() {  
  unsigned long cur = micros();
  unsigned long dif = cur - lastPulseTime; // 前回のエッジとの差分
  pulseInterval = (pulseInterval - (pulseInterval >> 2)) + (dif >> 2); // 滑らかに
  lastPulseTime = cur;
  //pmcount++;
}
//タイムアウトしたらrpm初期化
//フラグを下す(回転開始していない＆脱調していない)
bool PHOTO_CheckTimeout(){
  if(micros() - lastPulseTime > 100000){
    TRACE();
    rpm = 0;
    pulseInterval=100000000;
    inFlag=false;
    outFlag=false;
    return true;
  }else{
    return false;
  }
}

//脱調判定
void PHOTO_CheckOutOfStep(){
  //回転開始(RPM変化量+*以上、直前RPM ***以下)
    if(diffRPM>PHOTO_StartRPMD && beforeRPM<PHOTO_LowRPM && !inFlag){
      inFlag=true;//回転開始フラグを立てる
      //offsetが初期値のときは、回転開始DUTYを記録
      if(offset == PHOTO_OffsetDutyInitValue){
        inDuty=PHOTO_CheckDuty();
        inRPM = nowRPM;//回転開始RPMを記録
      }
    }
    //rpm out check(diffRPM>PHOTO_LimitRPMDを脱調と判定)
    if(diffRPM>PHOTO_LimitRPMD && !outFlag){
      outFlag=true;//脱調フラグ立てる
      outDuty = PHOTO_CheckDuty();//脱調時のDUTYを記録
      outRPM = nowRPM;//脱調時のRPMを記録
      DUMP(diffRPM);
      TRACE();
    }
}
bool PHOTO_CheckOutFlag(){
  return outFlag;
}


//Duty直接設定
void PHOTO_SetDuty(int d){
  duty = d;
}
//現在のDutyを返す
int PHOTO_CheckDuty(){
  return duty;
}

//オフセットDUTYをセットする
void PHOTO_SetOffsetDuty(int d){
  offset = d;
}
//PID制御によるduty算出
void PHOTO_ClacDuty(uint32_t deltaTime){
      P = TargetRPM - nowRPM;
      I += P * deltaTime;
      D = (P - preP)/deltaTime;
      preP = P;
      //duty += Kp * P + Kd * D + Ki * I;
      //float duty_f += Kp * P;
      duty_p += Kp * P + Kd * D;
      duty = duty_p + offset;

      //上下限すり切り,DUTY制限
      if(duty>=MAX_DUTY){
        duty=MAX_DUTY;
        duty_p=duty;
      }
      if(duty<MIN_DUTY){
        duty=MIN_DUTY;
        duty_p=duty;
      }

}

void PHOTO_IncreaseDuty(int d){
  duty+=d;
  if(duty>=MAX_DUTY)duty=MAX_DUTY;
}
void PHOTO_DecreaseDuty(int d){
  duty-=d;
  if(duty<=MIN_DUTY)duty=MIN_DUTY;
}
