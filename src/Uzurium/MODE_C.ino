const int MODE_C_TaskSpan = 100; // タスク実行間隔(ms)

uint32_t MODE_C_startTime = 0;//経過時間
uint32_t MODE_C_cycleTime = 0;//サイクルタイム
//uint32_t MODE_C_deltaTime = millis() - MODE_C_cycleTime;
//uint32_t MODE_C_spentTime = millis() - MODE_C_startTime;
bool MODE_C_Initialized = false;

int MODE_C_counter=0;//フォトリフレクタカウンター

void MODE_C_CountUp(){
  MODE_C_counter++;
}
//初期化処理
void MODE_C_Init(){
    //startTimeに現在時刻を設定
    MODE_C_startTime = millis();
    //サイクルタイムに現在時刻を設定
    MODE_C_cycleTime = millis();
    //duty = d;
    //TargetRPMを0に設定
    //PHOTO_Reset();
    MODE_C_counter=0;
    //Serialに表題情報をセット
    SERIAL_SetCheckIndex();
    //初期化フラグを立てる
    MODE_C_Initialized = true;
    //
    led.setbrightness(10);
    TRACE();
}
//終了処理
void MODE_C_Finish(){
    //初期化フラグを下す
    MODE_C_Initialized = false;
    //TargetRPMを0にセットする
    PHOTO_Reset();

    //PID制御によりDUTYを計算する
    PHOTO_SetDuty(0);
    //算出したDUTYでモータを回す
    motor.move(PHOTO_CheckDuty());
    //MODE_STOPにセットする
    //Uzurium_SetMode(MODE_STOP);
    //
    led.setbrightness(25);

    TRACE();
}
//ステータスチェック
bool MODE_C_CheckInit(){
  return MODE_C_Initialized;
}
//経過時間チェック
uint32_t MODE_C_CheckSpentTime(){
  return millis() - MODE_C_startTime;
}
//メイン関数
void MODE_C_main(){

  if(!MODE_C_Initialized){
    //未初期化のとき
    MODE_C_Init();
  }else{
    //初期化済みの時
    //ledをカウント値に合わせて光らせる
    led.counter(MODE_C_counter);
    //前回実行時からの経過時間を計算
    uint32_t deltaTime = millis() - MODE_C_cycleTime;
    //経過時間を計算
    uint32_t spentTime = millis() - MODE_C_startTime;
    //所定時間以上経過していたら実行
    if(deltaTime >= MODE_C_TaskSpan){
      //RPMを計測する
      PHOTO_CalcNowRPM();
      //シリアル通信のデータを書き出す
      SERIAL_SetSerialData();
      //エッジがしばらく来ない場合にRPM初期化
      PHOTO_CheckTimeout();
      //PID制御によりDUTYを計算する
      PHOTO_ClacDuty(deltaTime);
      //算出したDUTYでモータを回す
      //motor.move(PHOTO_CheckDuty());
      //モータ停止
      motor.move(0);
      //脱調判定
      //PHOTO_CheckOutOfStep();
      //音量を確認
      //int mag = FFT_CheckMagnitude();
      //mag = 30*mag;
      //OutRPMより大きくしないように調整
      //if(mag > PHOTO_CheckOutRPM(-100))mag = PHOTO_CheckOutRPM(-100);
      //TargetRPM設定
      if(spentTime>0)PHOTO_SetTargetRPM(0);
      //if(spentTime>200)PHOTO_SetTargetRPM(1000);
      //if(spentTime>200)PHOTO_SetTargetRPM(mag);
      //脱調していたら
      //if(PHOTO_CheckOutFlag()){
        //Serial.println("------DACCHO!-------");
        
        //MODE_Cを抜ける
      //  Uzurium_SetMode(MODE_STOP);
      //}
      //cycleTimeリセット
      MODE_C_cycleTime = millis();
    }
  }
}

