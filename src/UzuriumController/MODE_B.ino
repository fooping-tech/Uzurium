bool MODE_B_Initialized = false;
//初期化処理
void MODE_B_Init(){

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
        for(int i=1;i<=5;i++) send_data(1,i,duty,100+x,25+y);
      }
      else{
        if(BUTTON_check_blue())TRACE();
        if(BUTTON_check_red())TRACE();
        for(int i=1;i<=5;i++) send_data(1,i,0,10,25);
      }
    }
    
  }
}