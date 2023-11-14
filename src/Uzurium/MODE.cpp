#include "MODE.h"


uint32_t MODE::CheckSpentTime(){
  return spentTime;
}
void MODE::mainloop(){
  //前回実行時からの経過時間を計算
  deltaTime = millis() - cycleTime;
  //経過時間を計算
  spentTime = millis() - startTime;


  if(deltaTime >= TaskSpan){
    if(!_InitCondition){
      InitFunction();
    }else{
      main();
    }
    //cycleTimeリセット
    cycleTime = millis();
  }
}

void MODE::InitFunction(){
  motor->move(10);
  led->counter(_InitCounter);
  _InitCounter++;
  if(_InitMode==0){
    if(spentTime>0)motor->changeFreq(8);
    if(spentTime>100)motor->changeFreq(12);
    if(spentTime>200)motor->changeFreq(11);
    if(spentTime>300)motor->changeFreq(10);
    if(spentTime>400)motor->changeFreq(9);
    if(spentTime>500)motor->changeFreq(8);
    if(spentTime>600){
      motor->move(0);
      _InitCondition =true;
    }
  }

  if(_InitMode==1){
    if(spentTime>0)motor->changeFreq(8);
    if(spentTime>100)motor->changeFreq(9);
    if(spentTime>200)motor->changeFreq(10);
    if(spentTime>300)motor->changeFreq(11);
    if(spentTime>400)motor->changeFreq(12);
    if(spentTime>500)motor->changeFreq(8);
    if(spentTime>600){
      motor->move(0);
      _InitCondition =true;
    }
  }
  if(_InitMode==2){
    if(spentTime>0)motor->changeFreq(8);
    if(spentTime>100)motor->changeFreq(12);
    if(spentTime>200)motor->changeFreq(9);
    if(spentTime>300)motor->changeFreq(12);
    if(spentTime>400)motor->changeFreq(9);
    if(spentTime>500)motor->changeFreq(12);
    if(spentTime>600)motor->changeFreq(9);
    if(spentTime>700)motor->changeFreq(12);
    if(spentTime>800)motor->changeFreq(8);
    if(spentTime>900){
      motor->move(0);
      _InitCondition =true;
    }
  }
  if(_InitMode==3){
    if(spentTime>0)motor->changeFreq(8);
    if(spentTime>100)motor->changeFreq(9);
    if(spentTime>200){
      motor->changeFreq(10);
    }
    if(spentTime>300){
      motor->move(0);
    }
    if(spentTime>400){
      motor->move(10);
      motor->changeFreq(11);
    }
    if(spentTime>500)motor->changeFreq(12);
    if(spentTime>600)motor->changeFreq(13);
    if(spentTime>700){
      motor->move(0);
      _InitCondition =true;
    }
  }
  if(_InitMode==4){
    if(spentTime>0){
      motor->changeFreq(12);
    }
    if(spentTime>100)motor->move(0);
    if(spentTime>200){
      motor->move(10);
      motor->changeFreq(10);
    }
    if(spentTime>300){
      motor->changeFreq(11);
    }
    if(spentTime>400){
      motor->changeFreq(8);
    }
    if(spentTime>500){
      motor->move(0);
      _InitCondition =true;
    }
  }
  if(_InitMode==5){
    if(spentTime>0)motor->changeFreq(12);
    if(spentTime>100)motor->changeFreq(8);
    if(spentTime>200)motor->changeFreq(12);
    if(spentTime>300)motor->changeFreq(8);
    if(spentTime>400)motor->changeFreq(12);
    if(spentTime>500)motor->changeFreq(8);
    if(spentTime>600){
      motor->move(0);
      _InitCondition =true;
    }
  }
}
void MODE::main(){
      
}

