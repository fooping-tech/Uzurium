const int BUZZER_TaskSpan = 10; // タスク実行間隔(ms)

uint32_t BUZZER_startTime = 0;//経過時間
uint32_t BUZZER_cycleTime = 0;//サイクルタイム
//uint32_t BUZZER_deltaTime = millis() - BUZZER_cycleTime;
//uint32_t BUZZER_spentTime = millis() - BUZZER_startTime;
bool BUZZER_Initialized = false;
int BUZZER_pattern = 0;
int BUZZER_counter= 0;
//初期化処理
//pat:0:起動音,1:終了音
void BUZZER_On(int pat){
    if(!BUZZER_Initialized){
      //startTimeに現在時刻を設定
      BUZZER_startTime = millis();
      //サイクルタイムに現在時刻を設定
      BUZZER_cycleTime = millis();
      //初期化フラグを立てる
      BUZZER_Initialized = true;
      //モータが回らない程度にPWM
      if(BUZZER_pattern==0 || BUZZER_pattern==1)motor.move(20);

      BUZZER_pattern = pat;
      DUMP(BUZZER_pattern);
    }
}
//終了処理
void BUZZER_Finish(){
    //初期化フラグを下す
    BUZZER_Initialized = false;
    //モータ止める
    if(BUZZER_pattern==0 || BUZZER_pattern==1)motor.move(0);
    TRACE();
}
//ステータスチェック
bool BUZZER_CheckInit(){
  return BUZZER_Initialized;
}
//経過時間チェック
uint32_t BUZZER_CheckSpentTime(){
  return millis() - BUZZER_startTime;
}
//メイン関数
void BUZZER_main(){
  if(!BUZZER_Initialized){
    //初期化してなければなにもしない
  }else{
    //初期化済みの時
    //前回実行時からの経過時間を計算
    uint32_t deltaTime = millis() - BUZZER_cycleTime;
    //経過時間を計算
    uint32_t spentTime = millis() - BUZZER_startTime;
      //所定時間以上経過していたら実行
    if(deltaTime >= BUZZER_TaskSpan){
      if(BUZZER_pattern==0){
        if(spentTime>0)motor.changeFreq(8);
        if(spentTime>100)motor.changeFreq(9);
        if(spentTime>200)motor.changeFreq(10);
        if(spentTime>300)motor.changeFreq(11);
        if(spentTime>400)motor.changeFreq(12);
        if(spentTime>500)motor.changeFreq(8);
        if(spentTime>600)BUZZER_Finish();
      }
      if(BUZZER_pattern==1){
        DUMP(spentTime);
        if(spentTime>0)motor.changeFreq(8);
        if(spentTime>100)motor.changeFreq(12);
        if(spentTime>200)motor.changeFreq(11);
        if(spentTime>300)motor.changeFreq(10);
        if(spentTime>400)motor.changeFreq(9);
        if(spentTime>500)motor.changeFreq(8);
        if(spentTime>600)BUZZER_Finish();
      }
      if(BUZZER_pattern==3){
        if(spentTime>0)motor.changeFreq(8);
        if(spentTime>100)motor.changeFreq(9);
        if(spentTime>200)motor.changeFreq(10);
        if(spentTime>300)motor.changeFreq(11);
        if(spentTime>400)motor.changeFreq(12);
        if(spentTime>500)motor.changeFreq(8);
        if(spentTime>600)BUZZER_Finish();
      }
      led.counter(BUZZER_counter);
      BUZZER_counter++;
      //所定時間タスク終了後サイクルタイム初期化
      BUZZER_cycleTime = millis();

    }
  }
}


