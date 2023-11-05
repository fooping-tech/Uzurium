const int MODE_A_TaskSpan = 100; // タスク実行間隔(ms)

uint32_t MODE_A_startTime = 0;//経過時間
uint32_t MODE_A_Timer1 = 0;//タイマー
uint32_t MODE_A_Timer2 = 0;//タイマー
//uint32_t MODE_A_deltaTime = millis() - MODE_A_Timer1;
//uint32_t MODE_A_spentTime = millis() - MODE_A_startTime;


int MODE_A_CountY=1;
int MODE_A_PlusX=1;
int MODE_A_MinusX=9;

void MODE_A_ResetSQ(){
  MODE_A_CountY=1;
  MODE_A_PlusX=1;
  MODE_A_MinusX=9;
}

bool MODE_A_Initialized = false;

// MODE_A_Uzu構造体を定義
struct MODE_A_Uzu {
  int id=0;
  int x=0;
  int y=0;
  int hue=0;
  int duty=0;
  int brightness=0;

  void send(){
    send_data(1,id,duty,hue,brightness);
  }
};
MODE_A_Uzu myMODE_A_Uzu[5];

// モードの定義
enum MODE_A_SQMode {
  MODE_A_ALL,
  MODE_A_YFRONT,
  MODE_A_XLEFT,
  MODE_A_XRIGHT
};

MODE_A_SQMode MODE_A_currentMode = MODE_A_ALL; // 初期モードをALLに設定

//初期化処理
void MODE_A_Init(){
    //startTimeに現在時刻を設定
    MODE_A_startTime = millis();
    //タイマーに現在時刻を設定
    MODE_A_Timer1 = millis();
    //タイマーに現在時刻を設定
    MODE_A_Timer2 = millis();
    //初期化フラグを立てる
    MODE_A_Initialized = true;
    DISP_PictWrite();
    M5.Lcd.fillCircle(45, 240-40, 5, RED);
    TRACE();

    myMODE_A_Uzu[0].id=1;
    myMODE_A_Uzu[0].x=3;
    myMODE_A_Uzu[0].y=1;
    myMODE_A_Uzu[1].id=2;
    myMODE_A_Uzu[1].x=2;
    myMODE_A_Uzu[1].y=2;
    myMODE_A_Uzu[2].id=3;
    myMODE_A_Uzu[2].x=4;
    myMODE_A_Uzu[2].y=2;
    myMODE_A_Uzu[3].id=4;
    myMODE_A_Uzu[3].x=1;
    myMODE_A_Uzu[3].y=3;
    myMODE_A_Uzu[4].id=5;
    myMODE_A_Uzu[4].x=5;
    myMODE_A_Uzu[4].y=3;
    

}
//終了処理
void MODE_A_Finish(){
    //初期化フラグを下す
    MODE_A_Initialized = false;
    TRACE();
}

void MODE_A_AllSQ(int duty,int hue ,int brightness){
      for(int i=0;i<5;i++){
        myMODE_A_Uzu[i].duty = duty;
        myMODE_A_Uzu[i].hue = hue;
        myMODE_A_Uzu[i].brightness = brightness;
        myMODE_A_Uzu[i].send();
      }

}
void MODE_A_YFRONTSQ(int duty,int hue ,int brightness){
      for(int i=0;i<5;i++){
        if(MODE_A_CountY >= myMODE_A_Uzu[i].y && myMODE_A_Uzu[i].y >= MODE_A_CountY -1 ){
          myMODE_A_Uzu[i].duty = 75;
          myMODE_A_Uzu[i].hue = hue+50;
          myMODE_A_Uzu[i].brightness = brightness;
          myMODE_A_Uzu[i].send();
        }
        else{
          myMODE_A_Uzu[i].duty = 0;
          myMODE_A_Uzu[i].hue = hue;
          myMODE_A_Uzu[i].brightness = brightness;
          myMODE_A_Uzu[i].send();
        }
      }
      
}
void MODE_A_XleftSQ(int duty,int hue ,int brightness){
      for(int i=0;i<5;i++){
      if(MODE_A_PlusX >= myMODE_A_Uzu[i].x && myMODE_A_Uzu[i].x >= MODE_A_PlusX -5 ){
        myMODE_A_Uzu[i].duty = 75;
        myMODE_A_Uzu[i].hue = hue+50;
        myMODE_A_Uzu[i].brightness = brightness;
        myMODE_A_Uzu[i].send();
      }
      else{
        myMODE_A_Uzu[i].duty = 0;
        myMODE_A_Uzu[i].hue = hue;
        myMODE_A_Uzu[i].brightness = brightness;
        myMODE_A_Uzu[i].send();
      }
    }
    
}
void MODE_A_XRIGHTSQ(int duty,int hue ,int brightness){
      for(int i=0;i<5;i++){
      if(MODE_A_MinusX >= myMODE_A_Uzu[i].x && myMODE_A_Uzu[i].x >= MODE_A_MinusX -5 ){
        myMODE_A_Uzu[i].duty = 75;
        myMODE_A_Uzu[i].hue = hue+50;
        myMODE_A_Uzu[i].brightness = brightness;
        myMODE_A_Uzu[i].send();
      }
      else{
        myMODE_A_Uzu[i].duty = 0;
        myMODE_A_Uzu[i].hue = hue;
        myMODE_A_Uzu[i].brightness = brightness;
        myMODE_A_Uzu[i].send();
      }
    }
}
void MODE_A_main(){
  if(!MODE_A_Initialized){
    //未初期化のとき
    MODE_A_Init();
  }else{
    //
    uint32_t CountTime1 = millis() - MODE_A_Timer1;
    //経過時間を計算
    uint32_t spentTime = millis() - MODE_A_startTime;

    int duty = 0;
    int value = analogRead(ANALOG_VR_PIN);
    value = map(value,4095,0,0,2000);
    duty = 75;
    int brightness = map(value,0,2000,25,75);
    //DUMP(duty);
    //Serial.println(duty);

    //タッチされたらduty=0で脱腸解除
    if (M5.Touch.isEnabled()) {
    auto t = M5.Touch.getDetail();
    auto x = t.distanceX();
    auto y = t.distanceY();
    auto p = t.isPressed();
    if(p==1)duty=0;
    }

    //小dutyは無視
    if(duty < 15){
      duty=0;
      hue=0;
      brightness=20;
      MODE_A_Timer1 = millis();
      MODE_A_currentMode = MODE_A_ALL;
      MODE_A_ResetSQ();
      TRACE();

    }
    //大dutyをトリガに色を変更
    if(duty>100){
      hue += 50;
    }
    if(duty>90){
        MODE_A_currentMode = MODE_A_ALL;
        MODE_A_ResetSQ();
        TRACE();
    }
    if(CountTime1/1000 > 5){
      //hue += 50;
      MODE_A_Timer1 =millis();
    }
    M5.Display.setCursor(0, 0);
    M5.Display.printf("duty = %4d, hue = %4d, bright = %4d",duty,hue,brightness);
    M5.Display.endWrite();

    //所定時間経過したら
    if(millis() - MODE_A_Timer1 > 5000 && MODE_A_currentMode == MODE_A_ALL){
      //モード切り替え
      MODE_A_currentMode = MODE_A_YFRONT;
      TRACE();
      MODE_A_ResetSQ();
      //タイマーリセット
      MODE_A_Timer1 = millis();
    }

    //所定時間経過したら
    if(millis() - MODE_A_Timer2 > 1500){
      //Counter処理
      if(MODE_A_currentMode == MODE_A_YFRONT){
        MODE_A_CountY++;
        if(MODE_A_CountY>4){
          MODE_A_ResetSQ();
          MODE_A_currentMode = MODE_A_XLEFT;
          TRACE();
        }
      }
      if(MODE_A_currentMode == MODE_A_XLEFT){
        MODE_A_PlusX++;
        if(MODE_A_PlusX>10){
          MODE_A_ResetSQ();
          MODE_A_currentMode = MODE_A_XRIGHT;
          TRACE();
        }
      }
      if(MODE_A_currentMode == MODE_A_XRIGHT){
        MODE_A_MinusX--;
        if(MODE_A_MinusX<0){
          MODE_A_ResetSQ();
          MODE_A_currentMode = MODE_A_ALL;        
          TRACE();
        }
      }
      hue += 50;
      //タイマーリセット
      MODE_A_Timer2 = millis();
    }
    DUMP(MODE_A_currentMode);
    switch(MODE_A_currentMode){
      case MODE_A_ALL:
        MODE_A_AllSQ(duty,hue,brightness);
        break;
      case MODE_A_YFRONT:
        MODE_A_YFRONTSQ(duty,hue,brightness);
        break;
      case MODE_A_XLEFT:
        MODE_A_XleftSQ(duty,hue,brightness);
        break;
      case MODE_A_XRIGHT:
        MODE_A_XRIGHTSQ(duty,hue,brightness);
        break;
    }
//    send_data(1,1,duty,hue,brightness);
  }
}