const int MODE_A_TaskSpan = 50; // タスク実行間隔(ms)

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
    PHOTO_Set_TargetRPM(0);
    //Serialに表題情報をセット
    SERIAL_SetCheckIndex();
    //初期化フラグを立てる
    MODE_A_Initialized = true;
}
//終了処理
void MODE_A_Finish(){
    //初期化フラグを下す
    MODE_A_Initialized = false;
    //TargetRPMを0にセットする
    PHOTO_Set_TargetRPM(0);
    //MODE_STOPにセットする
    Uzurium_SetMode(MODE_STOP);
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
      SPEED_ClacDuty(deltaTime);
      //算出したDUTYでモータを回す
      motor.move(SPEED_CheckDuty());
      //脱調判定
      PHOTO_check();
      //TargetRPM設定
      if(spentTime>0)PHOTO_Set_TargetRPM(0);
      if(spentTime>200)PHOTO_Set_TargetRPM(1000);
      if(spentTime>1000)PHOTO_Set_TargetRPM(PHOTO_Check_outRPM());

      //脱調していたら
      if(PHOTO_GetOutFlag()){
        //MODE_Aを抜ける
        MODE_A_Finish();
      }
      //cycleTimeリセット
      MODE_A_cycleTime = millis();
    }
  }
}

