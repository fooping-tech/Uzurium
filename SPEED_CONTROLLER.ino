//PID
float P,I,D,preP;
int offset=100;//ugokidashi duty
float duty_p=0;
int duty=0;

//Duty直接設定
void SPEED_SetDuty(int d){
  duty = d;
}
//現在のDutyを返す
int SPEED_CheckDuty(){
  return duty;
}

//PID制御によるduty算出
void SPEED_ClacDuty(uint32_t deltaTime){
      P = TargetRPM - nowRPM;
      I += P * deltaTime;
      D = (P - preP)/deltaTime;
      preP = P;

      //duty += Kp * P + Kd * D + Ki * I;
      //float duty_f += Kp * P;
      duty_p += Kp * P + Kd * D;
      duty = duty_p + offset;

      //上下限すり切り
      if(duty>=256)duty=256;
      if(duty<0)duty=0;

}
/*
bool SQ =false;
bool CheckSQ(){
  return SQ;
}
void SPEED_StartDuty(int d){
  SQ=true;
  startTime=millis();
  cycleTime=millis();
  //duty = d;
  PHOTO_Set_TargetRPM(0);
   //Serial set
  SERIAL_SetCheckIndex();
}
*/
void IncreaseDuty(int d){
  duty+=d;
  if(duty>=256)duty=256;
}
void DecreaseDuty(int d){
  duty-=d;
  if(duty<=0)duty=0;
}

// void StopDuty(){
//   SQ=false;
//   //duty =0;
//   TargetRPM=0;
// }

