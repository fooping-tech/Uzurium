#include "MODE.h"


uint32_t MODE::CheckSpentTime(){
  return 0;
}
void MODE::mainloop(){
  //前回実行時からの経過時間を計算
  deltaTime = millis() - cycleTime;
  //経過時間を計算
  spentTime = millis() - startTime;

  if(deltaTime >= TaskSpan){
      main();
      //cycleTimeリセット
      cycleTime = millis();
  }
}
  
void MODE::main(){
      
}
