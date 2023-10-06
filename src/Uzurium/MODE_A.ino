const int MODE_A_TaskSpan = 100; // タスク実行間隔(ms)

uint32_t MODE_A_startTime = 0;//経過時間
uint32_t MODE_A_cycleTime = 0;//サイクルタイム
//uint32_t MODE_A_deltaTime = millis() - MODE_A_cycleTime;
//uint32_t MODE_A_spentTime = millis() - MODE_A_startTime;
bool MODE_A_Initialized = false;


//初期化処理
void MODE_A_Init(){
    //startTimeに現在時刻を設定
    MODE_A_startTime = millis();
    //サイクルタイムに現在時刻を設定
    MODE_A_cycleTime = millis();
    //duty = d;
    //TargetRPMを0に設定
    //PHOTO_Reset();

    //Serialに表題情報をセット
    SERIAL_SetCheckIndex();
    //初期化フラグを立てる
    MODE_A_Initialized = true;
    //
    TRACE();
}
//終了処理
void MODE_A_Finish(){
    //初期化フラグを下す
    MODE_A_Initialized = false;
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
bool MODE_A_CheckInit(){
  return MODE_A_Initialized;
}
//経過時間チェック
uint32_t MODE_A_CheckSpentTime(){
  return millis() - MODE_A_startTime;
}
//メイン関数
void MODE_A_main(){

  if(!MODE_A_Initialized){
    //未初期化のとき
    MODE_A_Init();
  }else{
    //初期化済みの時
    //前回実行時からの経過時間を計算
    uint32_t deltaTime = millis() - MODE_A_cycleTime;
    //経過時間を計算
    uint32_t spentTime = millis() - MODE_A_startTime;
    //所定時間以上経過していたら実行
    if(deltaTime >= MODE_A_TaskSpan){
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
      //ポテンショ対応;DUTYを上書き
      motor.move(map(FFT_ADvalue,0,4095,0,200));
      //脱調判定
      PHOTO_CheckOutOfStep();
      //TargetRPM設定
/*
      if(spentTime>0)PHOTO_SetTargetRPM(0);
      if(spentTime>200)PHOTO_SetTargetRPM(500);
      if(spentTime>15000)PHOTO_SetTargetRPM(500);
      if(spentTime>25000)PHOTO_SetTargetRPM(3000);
      if(spentTime>30000)PHOTO_SetTargetRPM(PHOTO_CheckOutRPM(-100));//TargetRPMを脱調時RPMに設定(引数はマージンRPM)
      if(spentTime>40000)PHOTO_SetTargetRPM(1000);//TargetRPMを脱調時RPMに設定(引数はマージンRPM)
      if(spentTime>50000)PHOTO_SetTargetRPM(3000);//TargetRPMを脱調時RPMに設定(引数はマージンRPM)
      if(spentTime>60000)PHOTO_SetTargetRPM(1000);//TargetRPMを脱調時RPMに設定(引数はマージンRPM)
      if(spentTime>70000)PHOTO_SetTargetRPM(3000);//TargetRPMを脱調時RPMに設定(引数はマージンRPM)
      if(spentTime>80000)PHOTO_SetTargetRPM(6000);//TargetRPMを脱調時RPMに設定(引数はマージンRPM)
      if(spentTime>90000)PHOTO_SetTargetRPM(1000);//TargetRPMを脱調時RPMに設定(引数はマージンRPM)
*/
      //脱調していたら
      if(PHOTO_CheckOutFlag()){
        
        //MODE_Aを抜ける
        Uzurium_SetMode(MODE_STOP);
      }
      //cycleTimeリセット
      MODE_A_cycleTime = millis();
    }
  }
}

