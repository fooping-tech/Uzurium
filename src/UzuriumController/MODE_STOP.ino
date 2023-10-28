bool MODE_STOP_Initialized = false;
//初期化処理
void MODE_STOP_Init(){

    //初期化フラグを立てる
    MODE_STOP_Initialized = true;
    DISP_PictWrite();
    M5.Lcd.fillCircle(45, 240-40, 5, RED);
    TRACE();

}
//終了処理
void MODE_STOP_Finish(){
    //初期化フラグを下す
    MODE_STOP_Initialized = false;
}

void MODE_STOP_main(){
  if(!MODE_STOP_Initialized){
    //未初期化のとき
    MODE_STOP_Init();
  }else{
    //
  }
}