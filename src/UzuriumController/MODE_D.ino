bool MODE_D_Initialized = false;
uint32_t MODE_D_Timer1 = 0;//タイマー
int MODE_D_duty = 0;
//初期化処理
void MODE_D_Init(){

    //初期化フラグを立てる
    MODE_D_Initialized = true;
    DISP_PictWrite();
    M5.Lcd.setTextDatum(1);                                 // 上中央基準
    M5.Lcd.drawString("PUSH BLUE BUTTON", M5.Lcd.width()/2, 40, 4); 
    MODE_D_duty =0;
    MODE_D_Timer1 =millis();
    TRACE();
}
//終了処理
void MODE_D_Finish(){
    //初期化フラグを下す
    MODE_D_Initialized = false;
}
//ステータスチェック
bool MODE_D_CheckInit(){
  return MODE_D_Initialized;
}

void MODE_D_main(){
   if(!MODE_D_Initialized){
    //未初期化のとき
    MODE_D_Init();
  }else{
    if(millis() - MODE_D_Timer1 > 400){
      MODE_D_duty -= 1;
      if(MODE_D_duty<0)MODE_D_duty = 0;
    }
    if(BUTTON_check_blue())MODE_D_duty +=5;
    for(int i=1;i<=5;i++) send_data(1,i,MODE_D_duty,100+MODE_D_duty,25);
//      if(BUTTON_check_red())for(int i=1;i<=5;i++) send_data(1,i,0,100,25);
    //M5.Lcd.setTextDatum(1);                                 // 上中央基準
    //M5.Lcd.drawString("POWER:" + MODE_D_duty, M5.Lcd.width()/2, 40, 4);
    DUMP(MODE_D_duty);
  }
}