bool MODE_B_Initialized = false;
uint32_t MODE_B_startTime = 0;//経過時間
uint32_t MODE_B_Timer1 = 0;//タイマー
uint32_t MODE_B_Timer2 = 0;//タイマー
//初期化処理
void MODE_B_Init(){
    //startTimeに現在時刻を設定
    MODE_B_startTime = millis();
    //タイマーに現在時刻を設定
    MODE_B_Timer1 = millis();
    //タイマーに現在時刻を設定
    MODE_B_Timer2 = millis();
    //初期化フラグを立てる
    MODE_B_Initialized = true;
    DISP_PictWrite();
    M5.Lcd.fillCircle(320/2, 240-40, 5, RED);
    TRACE();
}
//終了処理
void MODE_B_Finish(){
    //初期化フラグを下す
    MODE_B_Initialized = false;
}
//ステータスチェック
bool MODE_B_CheckInit(){
  return MODE_B_Initialized;
}

void MODE_B_main(){
   if(!MODE_B_Initialized){
    //未初期化のとき
    MODE_B_Init();
  }else{
    M5.Display.startWrite();
    if (M5.Touch.isEnabled()) {
      auto t = M5.Touch.getDetail();
      auto x = t.distanceX();
      auto y = t.distanceY();
      auto p = t.isPressed();
      M5.Display.setCursor(0, 0);
      M5.Display.printf("x = %4d, y = %4d, press = %d", x, y, p);
      M5.Display.endWrite();
      if(p==1){
        int duty = 50 + y;
        if(duty > 200)duty =200;
        if(duty < 0 )duty = 0;
        send_data(1,0,duty,100+x,25+y);//for IVS
        for(int i=1;i<=9;i++) send_data(1,i,duty,100+x,25+y);
        MODE_B_Timer1=millis();//for IVS
      }
      else{
        if(BUTTON_check_blue())TRACE();
        if(BUTTON_check_red())TRACE();
        send_data(1,0,0,10,25);
        for(int i=1;i<=9;i++) send_data(1,i,0,10,25);
      }
    }
    //所定時間経過したら
    if(millis() - MODE_B_Timer1 > 1500){
      FinishMode();
      SetMode(MODE_A);
    }
  }
}