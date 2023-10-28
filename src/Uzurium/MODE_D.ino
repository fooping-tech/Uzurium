const int MODE_D_TaskSpan = 10; // タスク実行間隔(ms)

uint32_t MODE_D_startTime = 0;//経過時間
uint32_t MODE_D_cycleTime = 0;//サイクルタイム
uint32_t MODE_D_OutTime = 0;//経過時間
const int OutStopTime = 300; //脱調後停止時間[ms]
//uint32_t MODE_D_deltaTime = millis() - MODE_D_cycleTime;
//uint32_t MODE_D_spentTime = millis() - MODE_D_startTime;
bool MODE_D_Initialized = false;


//初期化処理
void MODE_D_Init(){
    //startTimeに現在時刻を設定
    MODE_D_startTime = millis();
    //サイクルタイムに現在時刻を設定
    MODE_D_cycleTime = millis();
    //duty = d;
    //TargetRPMを0に設定
    //PHOTO_Reset();

    //Serialに表題情報をセット
    SERIAL_SetCheckIndex();
    //初期化フラグを立てる
    MODE_D_Initialized = true;
    //
    TRACE();
}
//終了処理
void MODE_D_Finish(){
    //初期化フラグを下す
    MODE_D_Initialized = false;
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
bool MODE_D_CheckInit(){
  return MODE_D_Initialized;
}
//経過時間チェック
uint32_t MODE_D_CheckSpentTime(){
  return millis() - MODE_D_startTime;
}
//メイン関数
void MODE_D_main(){

  if(!MODE_D_Initialized){
    //未初期化のとき
    MODE_D_Init();
  }else{
    //初期化済みの時
    //前回実行時からの経過時間を計算
    uint32_t deltaTime = millis() - MODE_D_cycleTime;
    //経過時間を計算
    uint32_t spentTime = millis() - MODE_D_startTime;
    //所定時間以上経過していたら実行
    if(deltaTime >= MODE_D_TaskSpan){
      //RPMを計測する
      PHOTO_CalcNowRPM();
      //シリアル通信のデータを書き出す
      SERIAL_SetSerialData();
      //エッジがしばらく来ない場合にRPM初期化
      PHOTO_CheckTimeout();
      //PID制御によりDUTYを計算する
      PHOTO_ClacDuty(deltaTime);
      //BUZZERを鳴らしていない時
      if(!BUZZER_CheckInit()){
        //算出したDUTYでモータを回す
        //motor.move(PHOTO_CheckDuty());
        //ポテンショ対応;DUTYを上書き
        //int duty = map(FFT_ADvalue,0,4095,0,200);
        motor.move(ESPNOW_CheckDuty());
        //脱調検出後指定時間はdutyを0にする
        if(OutStopTime > millis() - MODE_D_OutTime)motor.move(0);
        //DUTYに応じてLEDのhueを可変
        led.fire2(4,ESPNOW_CheckHue());
        led.setbrightness(ESPNOW_CheckBrightness());
        DUMP(ESPNOW_CheckDuty());
        DUMP(ESPNOW_CheckHue());
      }
      //脱調判定
      PHOTO_CheckOutOfStep();
      //TargetRPM設定

      //脱調していたら
      if(PHOTO_CheckOutFlag()){
        MODE_D_OutTime = millis();
      }
      //ポテンショの値をUzuriumNumとして記録
      Uzurium_Number = VR_CheckValue();
      //cycleTimeリセット
      MODE_D_cycleTime = millis();
    }
  }
}

