const int TIME_A_TaskSpan = 100; // タスク実行間隔(ms)

uint32_t TIME_A_startTime = 0;//経過時間
uint32_t TIME_A_cycleTime = 0;//サイクルタイム
//uint32_t TIME_A_deltaTime = millis() - TIME_A_cycleTime;
//uint32_t TIME_A_spentTime = millis() - TIME_A_startTime;
bool TIME_A_Initialized = false;


//初期化処理
void TIME_A_Init(){
    //startTimeに現在時刻を設定
    TIME_A_startTime = millis();
    //サイクルタイムに現在時刻を設定
    TIME_A_cycleTime = millis();
    //duty = d;
    //TargetRPMを0に設定
    //PHOTO_Reset();

    //初期化フラグを立てる
    TIME_A_Initialized = true;
    //
}
//終了処理
void TIME_A_Finish(){
    //初期化フラグを下す
    TIME_A_Initialized = false;
    //
    
}
//ステータスチェック
bool TIME_A_CheckInit(){
  return TIME_A_Initialized;
}
//経過時間チェック
uint32_t TIME_A_CheckSpentTime(){
  return millis() - TIME_A_startTime;
}
//メイン関数
void TIME_A_main(){
  
  if(!TIME_A_Initialized){
    //未初期化のとき
    TIME_A_Init();
  }else{
    //初期化済みの時
    //前回実行時からの経過時間を計算
    uint32_t deltaTime = millis() - TIME_A_cycleTime;
    //経過時間を計算
    uint32_t spentTime = millis() - TIME_A_startTime;

    
    //所定時間以上経過していたら実行
    if(deltaTime >= TIME_A_TaskSpan){
        //Main
        
        
        
        

      //cycleTimeリセット
      TIME_A_cycleTime = millis();
      }
    }
  }


