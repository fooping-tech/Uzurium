const int MODE_B_TaskSpan = 100; // タスク実行間隔(ms)

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
    PHOTO_Reset();

    //PID制御によりDUTYを計算する
    PHOTO_SetDuty(0);
    //算出したDUTYでモータを回す
    motor.move(PHOTO_CheckDuty());
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
      PHOTO_CalcNowRPM();
      //シリアル通信のデータを書き出す
      SERIAL_SetSerialData();
      //エッジがしばらく来ない場合にRPM初期化
      PHOTO_CheckTimeout();
      //PID制御によりDUTYを計算する
      PHOTO_ClacDuty(deltaTime);
      //算出したDUTYでモータを回す
      motor.move(PHOTO_CheckDuty());
      //脱調判定
      PHOTO_CheckOutOfStep();
      //音量を確認
      int mag = FFT_CheckMagnitude();
      mag = 30*mag;
      //OutRPMより大きくしないように調整
      if(mag > PHOTO_CheckOutRPM(-100))mag = PHOTO_CheckOutRPM(-100);
      //TargetRPM設定
      if(spentTime>0)PHOTO_SetTargetRPM(0);
      //if(spentTime>200)PHOTO_SetTargetRPM(1000);
      if(spentTime>200)PHOTO_SetTargetRPM(mag);
      //脱調していたら
      if(PHOTO_CheckOutFlag()){
        //Serial.println("------DACCHO!-------");
        
        //MODE_Bを抜ける
        Uzurium_SetMode(MODE_STOP);
      }
      //cycleTimeリセット
      MODE_B_cycleTime = millis();
    }
  }
}

