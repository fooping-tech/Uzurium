const int MODE_C_TaskSpan = 100; // タスク実行間隔(ms)

uint32_t MODE_C_startTime = 0;//経過時間
uint32_t MODE_C_Timer1 = 0;//タイマー
uint32_t MODE_C_Timer2 = 0;//タイマー
//uint32_t MODE_C_deltaTime = millis() - MODE_C_Timer1;
//uint32_t MODE_C_spentTime = millis() - MODE_C_startTime;


int CountY=1;
int PlusX=1;
int MinusX=9;

void MODE_C_ResetSQ(){
  CountY=1;
  PlusX=1;
  MinusX=9;
}

bool MODE_C_Initialized = false;

// Uzu構造体を定義
struct Uzu {
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
Uzu myUzu[5];

// モードの定義
enum SQMode {
  ALL,
  YFRONT,
  XLEFT,
  XRIGHT
};

SQMode currentMode = ALL; // 初期モードをALLに設定

//初期化処理
void MODE_C_Init(){
    //startTimeに現在時刻を設定
    MODE_C_startTime = millis();
    //タイマーに現在時刻を設定
    MODE_C_Timer1 = millis();
    //タイマーに現在時刻を設定
    MODE_C_Timer2 = millis();
    //初期化フラグを立てる
    MODE_C_Initialized = true;
    DISP_PictWrite();
    M5.Lcd.fillCircle(320-55, 240-40, 5, RED);
    TRACE();

    myUzu[0].id=1;
    myUzu[0].x=3;
    myUzu[0].y=1;
    myUzu[1].id=2;
    myUzu[1].x=2;
    myUzu[1].y=2;
    myUzu[2].id=3;
    myUzu[2].x=4;
    myUzu[2].y=2;
    myUzu[3].id=4;
    myUzu[3].x=1;
    myUzu[3].y=3;
    myUzu[4].id=5;
    myUzu[4].x=5;
    myUzu[4].y=3;
    

}
//終了処理
void MODE_C_Finish(){
    //初期化フラグを下す
    MODE_C_Initialized = false;
    TRACE();
}

void AllSQ(int duty,int hue ,int brightness){
      for(int i=0;i<5;i++){
        myUzu[i].duty = duty;
        myUzu[i].hue = hue;
        myUzu[i].brightness = brightness;
        myUzu[i].send();
      }

}
void YfrontSQ(int duty,int hue ,int brightness){
      for(int i=0;i<5;i++){
        if(CountY >= myUzu[i].y && myUzu[i].y >= CountY -1 ){
          myUzu[i].duty = 75;
          myUzu[i].hue = hue+50;
          myUzu[i].brightness = brightness;
          myUzu[i].send();
        }
        else{
          myUzu[i].duty = 0;
          myUzu[i].hue = hue;
          myUzu[i].brightness = brightness;
          myUzu[i].send();
        }
      }
      
}
void XleftSQ(int duty,int hue ,int brightness){
      for(int i=0;i<5;i++){
      if(PlusX >= myUzu[i].x && myUzu[i].x >= PlusX -5 ){
        myUzu[i].duty = 75;
        myUzu[i].hue = hue+50;
        myUzu[i].brightness = brightness;
        myUzu[i].send();
      }
      else{
        myUzu[i].duty = 0;
        myUzu[i].hue = hue;
        myUzu[i].brightness = brightness;
        myUzu[i].send();
      }
    }
    
}
void XrightSQ(int duty,int hue ,int brightness){
      for(int i=0;i<5;i++){
      if(MinusX >= myUzu[i].x && myUzu[i].x >= MinusX -5 ){
        myUzu[i].duty = 75;
        myUzu[i].hue = hue+50;
        myUzu[i].brightness = brightness;
        myUzu[i].send();
      }
      else{
        myUzu[i].duty = 0;
        myUzu[i].hue = hue;
        myUzu[i].brightness = brightness;
        myUzu[i].send();
      }
    }
}
void MODE_C_main(){
  if(!MODE_C_Initialized){
    //未初期化のとき
    MODE_C_Init();
  }else{
    //
    uint32_t CountTime1 = millis() - MODE_C_Timer1;
    //経過時間を計算
    uint32_t spentTime = millis() - MODE_C_startTime;

    int duty = 0;
    int value = analogRead(ANALOG_VR_PIN);
    value = map(value,4095,0,0,2000);
    int gain = map(value,2000,0,0,100);
    int mag = FFT_CheckMagnitude();
    duty = map(mag,0,value,0,200);
    int brightness = map(duty,0,200,25,75);
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
      MODE_C_Timer1 = millis();
      currentMode = ALL;
      MODE_C_ResetSQ();
      TRACE();

    }
    //大dutyをトリガに色を変更
    if(duty>100){
      hue += 50;
    }
    if(duty>90){
        currentMode = ALL;
        MODE_C_ResetSQ();
        TRACE();
    }
    if(CountTime1/1000 > 5){
      //hue += 50;
      MODE_C_Timer1 =millis();
    }
    M5.Display.setCursor(0, 0);
    M5.Display.printf("Gain = %4d, duty = %4d, hue = %4d, bright = %4d", gain,duty,hue,brightness);
    M5.Display.endWrite();

    //所定時間経過したら
    if(millis() - MODE_C_Timer1 > 5000 && currentMode == ALL){
      //モード切り替え
      currentMode = YFRONT;
      TRACE();
      MODE_C_ResetSQ();
      //タイマーリセット
      MODE_C_Timer1 = millis();
    }

    //所定時間経過したら
    if(millis() - MODE_C_Timer2 > 1500){
      //Counter処理
      if(currentMode == YFRONT){
        CountY++;
        if(CountY>4){
          MODE_C_ResetSQ();
          currentMode = XLEFT;
          TRACE();
        }
      }
      if(currentMode == XLEFT){
        PlusX++;
        if(PlusX>10){
          MODE_C_ResetSQ();
          currentMode = XRIGHT;
          TRACE();
        }
      }
      if(currentMode == XRIGHT){
        MinusX--;
        if(MinusX<0){
          MODE_C_ResetSQ();
          currentMode = ALL;        
          TRACE();
        }
      }
      hue += 50;
      //タイマーリセット
      MODE_C_Timer2 = millis();
    }
    DUMP(currentMode);
    switch(currentMode){
      case ALL:
        AllSQ(duty,hue,brightness);
        break;
      case YFRONT:
        YfrontSQ(duty,hue,brightness);
        break;
      case XLEFT:
        XleftSQ(duty,hue,brightness);
        break;
      case XRIGHT:
        XrightSQ(duty,hue,brightness);
        break;
    }
//    send_data(1,1,duty,hue,brightness);
  }
}