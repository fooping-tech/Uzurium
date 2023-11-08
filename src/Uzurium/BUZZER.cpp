#include "BUZZER.h"


//メイン関数
void BUZZER::main(){
    //初期化済みの時
    //前回実行時からの経過時間を計算
    uint32_t deltaTime = millis() - cycleTime;
    //経過時間を計算
    uint32_t spentTime = millis() - startTime;
      //所定時間以上経過していたら実行
    if(deltaTime >= TaskSpan){
      if(pattern==0){
        if(spentTime>0)motor.changeFreq(8);
        if(spentTime>100)motor.changeFreq(9);
        if(spentTime>200)motor.changeFreq(10);
        if(spentTime>300)motor.changeFreq(11);
        if(spentTime>400)motor.changeFreq(12);
        if(spentTime>500)motor.changeFreq(8);
        if(spentTime>600)finish();
      }
      if(pattern==1){
        if(spentTime>0)motor.changeFreq(8);
        if(spentTime>100)motor.changeFreq(12);
        if(spentTime>200)motor.changeFreq(11);
        if(spentTime>300)motor.changeFreq(10);
        if(spentTime>400)motor.changeFreq(9);
        if(spentTime>500)motor.changeFreq(8);
        if(spentTime>600)finish();
      }
      if(pattern==3){
        if(spentTime>0)motor.changeFreq(8);
        if(spentTime>100)motor.changeFreq(9);
        if(spentTime>200)motor.changeFreq(10);
        if(spentTime>300)motor.changeFreq(11);
        if(spentTime>400)motor.changeFreq(12);
        if(spentTime>500)motor.changeFreq(8);
        if(spentTime>600)finish();
      }
      led.counter(counter);
      counter++;
      //所定時間タスク終了後サイクルタイム初期化
      cycleTime = millis();

    }
  
}
