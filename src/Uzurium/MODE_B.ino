const int MODE_B_TaskSpan = 10; // タスク実行間隔(ms)

uint32_t MODE_B_startTime = 0;//経過時間
uint32_t MODE_B_cycleTime = 0;//サイクルタイム
//uint32_t MODE_B_deltaTime = millis() - MODE_B_cycleTime;
//uint32_t MODE_B_spentTime = millis() - MODE_B_startTime;
bool MODE_B_Initialized = false;


//初期化処理
void MODE_B_Init(){
    //startTimeに現在時刻を設定
    MODE_B_startTime = millis();
    //サイクルタイムに現在時刻を設定
    MODE_B_cycleTime = millis();
    //duty = d;
    //TargetRPMを0に設定
    //PHOTO_Reset();

    //Serialに表題情報をセット
    SERIAL_SetCheckIndex();
    //初期化フラグを立てる
    MODE_B_Initialized = true;
    //
    TRACE();
}
//終了処理
void MODE_B_Finish(){
    //初期化フラグを下す
    MODE_B_Initialized = false;
    //TargetRPMを0にセットする
    photo.reset();

    //PID制御によりDUTYを計算する
    photo.SetDuty(0);
    //算出したDUTYでモータを回す
    motor.move(photo.CheckDuty());
    //MODE_STOPにセットする
    //Uzurium_SetMode(MODE_STOP);
    //
    TRACE();
}
//ステータスチェック
bool MODE_B_CheckInit(){
  return MODE_B_Initialized;
}
//経過時間チェック
uint32_t MODE_B_CheckSpentTime(){
  return millis() - MODE_B_startTime;
}
//メイン関数
void MODE_B_main(){

  if(!MODE_B_Initialized){
    //未初期化のとき
    MODE_B_Init();
  }else{
    //初期化済みの時
    //前回実行時からの経過時間を計算
    uint32_t deltaTime = millis() - MODE_B_cycleTime;
    //経過時間を計算
    uint32_t spentTime = millis() - MODE_B_startTime;
    //所定時間以上経過していたら実行
    if(deltaTime >= MODE_B_TaskSpan){
      //RPMを計測する
      photo.CalcNowRPM();
      //シリアル通信のデータを書き出す
      SERIAL_SetSerialData();
      //エッジがしばらく来ない場合にRPM初期化
      photo.CheckTimeout();
      //BUZZERを鳴らしていない時
      if(!BUZZER_CheckInit()){
         //PID制御によりDUTYを計算する
        photo.ClacDuty(deltaTime);
        //算出したDUTYでモータを回す
        motor.move(photo.CheckDuty());
        //DUTYに応じてLEDのhueを可変
        led.fire2(4,photo.CheckDuty());
      }
      //Kpをポテンショで可変
      Kp = map(FFT_ADvalue,0,4095,0,1000) * 0.000001;
      Serial.print("Kp=");
      Serial.println(Kp,8);
      //脱調判定
      photo.CheckOutOfStep();
      //TargetRPM設定
      if(spentTime>1000)photo.SetTargetRPM(0);
      if(spentTime>3000) photo.SetTargetRPM(1000);
      if(spentTime>10000) photo.SetTargetRPM(1500);
      if(spentTime>15000)photo.SetTargetRPM(2000);
//      if(spentTime>30000)photo.SetTargetRPM(photo.CheckOutRPM(-100));//TargetRPMを脱調時RPMに設定(引数はマージンRPM)
      if(spentTime>20000)photo.SetTargetRPM(2500);//TargetRPMを脱調時RPMに設定(引数はマージンRPM)
      if(spentTime>25000)photo.SetTargetRPM(3000);//TargetRPMを脱調時RPMに設定(引数はマージンRPM)
      if(spentTime>30000)photo.SetTargetRPM(3000);//TargetRPMを脱調時RPMに設定(引数はマージンRPM)
      if(spentTime>35000)photo.SetTargetRPM(3000);//TargetRPMを脱調時RPMに設定(引数はマージンRPM)

      //脱調していたら
      if(photo.CheckOutFlag()){
        //Serial.println("------DACCHO!-------");
        
        //MODE_Bを抜ける
        Uzurium_SetMode(MODE_STOP);
      }
      //cycleTimeリセット
      MODE_B_cycleTime = millis();
    }
  }
}

