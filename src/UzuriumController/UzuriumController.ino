#include "./configuration.h"
#include "pict2.h"


//モード定義
enum Mode{
  MODE_STOP,
  MODE_A,
  MODE_B,
  MODE_C,
  MODE_D
};
Mode mode = MODE_STOP;
int hue = 0;
Button btn1(BTN1_PIN);
Button btn2(BTN2_PIN);
Button btn3(BTN3_PIN);
NEOPIXELLED led = NEOPIXELLED();

//MODEをセットする
void SetMode(Mode m){
  //実行中モードをFinishする
  mode = m;
}
void FinishMode(){
  MODE_STOP_Finish();
  MODE_A_Finish();
  MODE_B_Finish();
  MODE_C_Finish();
  MODE_D_Finish();
  TRACE();
  led.setbrightness(25);
}
//Uzurium 現在のMODEをCheckする
Mode CheckState(){
  return mode;
}

void setup(void)
{
  auto cfg = M5.config(); // 設定用の構造体を代入。
  cfg.serial_baudrate = 9600;
  cfg.output_power = true;
  M5.begin(cfg); // 設定した値でデバイスを開始する。

  ESPNOW_setup();
  BUTTON_setup();
  btn1.setup();
  btn2.setup();
  btn3.setup();
  led.setup(LED_PIN);
  //mac addr
  //Serial.println(WiFi.macAddress());
  //Core0 WDT無効化
  //disableCore0WDT();
  //Core0でタスク起動
  xTaskCreatePinnedToCore(
    Uzurium_Task        // タスク関数へのポインタ。無限ループで終了しないよう関数を指定します
    ,  "TaskMotion"   // タスクの説明用名前。重複しても動きますがデバッグ用途。最大16文字まで
    ,  4096  //スタックサイズ(Byte)
    ,  NULL //作成タスクのパラメータのポインタ
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest..  优先级，3 (configMAX_PRIORITIES - 1) 最高，0 最低。
    ,  NULL //作成タスクのHandleへのポインタ
    ,  0);  //利用するCPUコア(0-1)
  SetMode(MODE_STOP);
}
void Uzurium_Task(void *pvParameters){
  while(1){
    Uzurium_main();
    delay(1);
  }
}

void loop(void){
  //FFT
  FFT_main();

  //btnCheck
  if(BUTTON_check_red()||btn1.check()){
    send_data(0,1,0,0,0);//RemoteControlModeへ入れる
    FinishMode();
    SetMode(MODE_A);
  }
  if(btn2.check()){
    send_data(0,1,0,0,0);//RemoteControlModeへ入れる
    FinishMode();
    SetMode(MODE_B);
  }
  if(BUTTON_check_blue()||btn3.check()){
    send_data(0,1,0,0,0);//RemoteControlModeへ入れる
    FinishMode();
    SetMode(MODE_C);
  }


  //LED
  if(mode == MODE_A ||mode == MODE_STOP){
    led.pacifica();
  }
  if(mode == MODE_B){
    int duty = MODE_B_CheckDuty();
    duty = map(duty,0,160,0,360);
    int brightness = map(duty,0,360,1,80);
    led.setbrightness(brightness);
    led.fire2(4,duty);
  }
  if(mode == MODE_C){
    int value = analogRead(ANALOG_VR_PIN);
    value = map(value,4095,0,1,2000);
    int gain = map(value,2000,0,0,100);
    int mag = FFT_CheckMagnitude();
    int level = map(mag,0,value,0,11);
    DUMP(level);
    led.level2(level,level);


  }

}
bool CheckTouch(){
  if (M5.Touch.isEnabled()) {
      auto t = M5.Touch.getDetail();
      auto p = t.isPressed();
      DUMP(p);
      if(p==1)return true;
  }
  return false;
}

void Uzurium_main(void){
  M5.update();
  
  if(CheckTouch() && !MODE_B_CheckInit()){
    send_data(0,1,0,0,0);//RemoteControlModeへ入れる
    SetMode(MODE_B);
  }
  //SW
  if(M5.BtnA.wasPressed()){
    send_data(0,1,0,0,0);//RemoteControlModeへ入れる
    FinishMode();
    SetMode(MODE_A);
  }
  if(M5.BtnB.wasPressed()){
    send_data(0,1,0,0,0);//RemoteControlModeへ入れる
    FinishMode();
    SetMode(MODE_B);
  }
  if(M5.BtnC.wasPressed()){
    send_data(0,1,0,0,0);//RemoteControlModeへ入れる
    FinishMode();
    SetMode(MODE_C);
  }

  //MODE
  if(mode == MODE_STOP){
    MODE_STOP_main();
  }
  if(mode == MODE_A){
    MODE_A_main();
  }
  if(mode == MODE_B){
    MODE_B_main();
  }
  if(mode == MODE_C){
    MODE_C_main();
  }
  if(mode == MODE_D){
    MODE_D_main();
  }
}

