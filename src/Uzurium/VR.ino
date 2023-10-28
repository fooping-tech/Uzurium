const int VR_TaskSpan = 50; // タスク実行間隔(ms)

uint32_t VR_startTime = 0;//経過時間
uint32_t VR_cycleTime = 0;//サイクルタイム
//uint32_t VR_deltaTime = millis() - VR_cycleTime;
//uint32_t VR_spentTime = millis() - VR_startTime;
uint32_t VR_TrigTime = 0;//サイクルタイム

bool VR_Initialized = false;
int VR_pattern = 0;
int VR_counter= 0;

int preADvalue =0;
int VR_WaitTime = 2000;//ms
int VR_ChangeRange = 100;//ポテンショ変更判定閾値
int VR_value =0;
void VR_CheckChange(){
  if(abs(FFT_ADvalue - preADvalue) >VR_ChangeRange)VR_TrigTime=millis();
  preADvalue = FFT_ADvalue;
}


//初期化処理
//pat:0:起動音,1:終了音
void VR_Init(){
    if(!VR_Initialized){
      //startTimeに現在時刻を設定
      VR_startTime = millis();
      //サイクルタイムに現在時刻を設定
      VR_cycleTime = millis();
      //初期化フラグを立てる
      VR_Initialized = true;

      //VR_pattern = pat;
      //DUMP(VR_pattern);
    }
}
//終了処理
void VR_Finish(){
    //初期化フラグを下す
    VR_Initialized = false;
    //モータ止める
    if(VR_pattern==0 || VR_pattern==1)motor.move(0);
    TRACE();
}
//ステータスチェック
bool VR_CheckInit(){
  return VR_Initialized;
}
int VR_CheckValue(){
  return VR_value;
}
//経過時間チェック
uint32_t VR_CheckSpentTime(){
  return millis() - VR_startTime;
}
//メイン関数
void VR_main(){
  if(!VR_Initialized){
    //初期化してなければ初期化
    VR_Init();
  }else{
    //初期化済みの時
    //前回実行時からの経過時間を計算
    uint32_t deltaTime = millis() - VR_cycleTime;
    //経過時間を計算
    uint32_t spentTime = millis() - VR_startTime;
      //所定時間以上経過していたら実行
    if(deltaTime >= VR_TaskSpan){
      VR_CheckChange();
      if(VR_WaitTime > millis() - VR_TrigTime){
        VR_value = map(FFT_ADvalue,0,4095,1,18);
        for(int i=1 ;i<=VR_value;i++)led.counter(i);
        DUMP(VR_value);
      }
      //VRがない時用
      VR_value=4;
      //led.counter(VR_counter);
      //VR_counter++;
      //所定時間タスク終了後サイクルタイム初期化
      VR_cycleTime = millis();

    }
  }
}


