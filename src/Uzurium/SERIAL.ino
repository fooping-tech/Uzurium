/*
//表題情報設定状態;設定済:true 未設定:false
bool indexFlag=false;

void SERIAL_setup(){
  //SERIAL INITIAL
  Serial.begin(115200); //Serial begin
  delay(50);   //delay 50ms.  延迟50ms
  Serial.println("SERIAL setup was completed.");
}

//Serialに表題情報を書き込む
void SERIAL_SetCheckIndex(){
//  if(!indexFlag){  //一度だけ実行
  if(true){  //毎回実行
  //  Serial.print("t_ms");
  //  Serial.print(",");
  //  Serial.print("shuntvoltage");
  //  Serial.print(",");
  //  Serial.print("busvoltage");
  //  Serial.print(",");
  //  Serial.print("current_mA");
  //  Serial.print(",");
  //  Serial.print("current_mA[ave]");
  //  Serial.print(",");
  //  Serial.print("loadvoltage");
  //  Serial.print(",");
    Serial.print("rpm");
    Serial.print(",");
    Serial.print("rpm[ave]");
    Serial.print(",");
    Serial.print("rpm_D");
    Serial.print(",");
    Serial.print("rpm_Target");
    Serial.print(",");
    Serial.println("duty");
  }
  //set index flag on
  indexFlag=true;
}

//Serialにデータを書き込む
void SERIAL_SetSerialData(){
  //INA219
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;

//  shuntvoltage = ina219.getShuntVoltage_mV();
//  busvoltage = ina219.getBusVoltage_V();
//  current_mA = ina219.getCurrent_mA();
//  power_mW = ina219.getPower_mW();
//  loadvoltage = busvoltage + (shuntvoltage / 1000);

  uint32_t spentTime2 = millis();

//  Serial.print(spentTime2);
//  Serial.print(",");
//  Serial.print(shuntvoltage);
//  Serial.print(",");
//  Serial.print(busvoltage);
//  Serial.print(",");
//  Serial.print(current_mA);
//  Serial.print(",");
//  Serial.print(INA219_AverageCurrent(current_mA));  
//  Serial.print(",");
//  Serial.print(loadvoltage);
//  Serial.print(",");
//  detachInterrupt(digitalPinToInterrupt(PHOTO_PIN)); // 割り込みを一時的に解除
  photo.StopInterrupt();//割り込み解除
    photo.CalcRPM();// RPMを求める

    Serial.print(photo.CheckRpm());
    Serial.print(",");
    
    Serial.print(photo.CheckNowRPM());//rpm平均値
    Serial.print(",");
    Serial.print(photo.CheckDiffRPM());
  photo.SetInterrupt();//割り込み再設定
  //attachInterrupt(digitalPinToInterrupt(PHOTO_PIN), PHOTO_Measure, FALLING); // 割り込みを再設定
  Serial.print(",");
  Serial.print(photo.CheckTargetRPM());
  Serial.print(",");
  Serial.print(photo.CheckDuty_p());
  Serial.print(",");
  Serial.println(photo.CheckDuty());



  // if(spentTime>10000)TargetRPM=5000;
  // if(spentTime>14000)TargetRPM=1000;
  // if(spentTime>18000)TargetRPM=0;
  // if(spentTime>22000)TargetRPM=0;
    
}

//Serial入力をチェックする(int型のみ)
void SERIAL_InputCheck(){
  //シリアルが利用可能か確認
  if (Serial.available())
  {
    //値を読み取りrecに入れる
    int rec=Serial.parseInt();
    //Serial.print("rec=");
    //Serial.println(rec);

    if(rec>= 1){//改行コード0は無視し、1以上
      switch(rec){
        case 1:
          Uzurium_SetMode(MODE_STOP);
          break;
        case 2:
          Uzurium_SetMode(MODE_A);
          break;
        case 3:
          Uzurium_SetMode(MODE_B);
          break;
        case 4:
          Uzurium_SetMode(MODE_C);
          break;
        case 5:
          Uzurium_SetMode(MODE_D);
          break;
      }
    }
    //Serial.print("mode=");
    //Serial.println(mode);

  }else{
    //Serial通信切断時の処理
  }
}
*/