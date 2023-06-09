const int MODE_STOP_TaskSpan = 100; // タスク実行間隔(ms)

uint32_t MODE_STOP_startTime = 0;//経過時間
uint32_t MODE_STOP_cycleTime = 0;//サイクルタイム
//uint32_t MODE_STOP_deltaTime = millis() - MODE_STOP_cycleTime;
//uint32_t MODE_STOP_spentTime = millis() - MODE_STOP_startTime;
bool MODE_STOP_Initialized = false;


//初期化処理
void MODE_STOP_Init(){
    //startTimeに現在時刻を設定
    MODE_STOP_startTime = millis();
    //サイクルタイムに現在時刻を設定
    MODE_STOP_cycleTime = millis();
    //duty = d;
    //TargetRPMを0に設定
    //PHOTO_Reset();
    //Serialに表題情報をセット
    //SERIAL_SetCheckIndex();
    //初期化フラグを立てる
    MODE_STOP_Initialized = true;
    //
    TRACE();
}
//終了処理
void MODE_STOP_Finish(){
    //初期化フラグを下す
    MODE_STOP_Initialized = false;
    //TargetRPMを0にセットする
    //PHOTO_Reset();
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
bool MODE_STOP_CheckInit(){
  return MODE_STOP_Initialized;
}
//経過時間チェック
uint32_t MODE_STOP_CheckSpentTime(){
  return millis() - MODE_STOP_startTime;
}
//メイン関数
void MODE_STOP_main(){

  if(!MODE_STOP_Initialized){
    //未初期化のとき
    MODE_STOP_Init();
  }else{
    //初期化済みの時
    //前回実行時からの経過時間を計算
    uint32_t deltaTime = millis() - MODE_STOP_cycleTime;
    //経過時間を計算
    uint32_t spentTime = millis() - MODE_STOP_startTime;
    //所定時間以上経過していたら実行
    if(deltaTime >= MODE_STOP_TaskSpan){
      //PID制御によりDUTYを計算する
      PHOTO_SetDuty(0);
      //算出したDUTYでモータを回す
      motor.move(PHOTO_CheckDuty());
      //cycleTimeリセット
      MODE_STOP_cycleTime = millis();
    }
  }
}

