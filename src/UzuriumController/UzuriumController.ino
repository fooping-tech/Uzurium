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

}
//Uzurium 現在のMODEをCheckする
Mode CheckState(){
  return mode;
}

void setup(void)
{
  M5.begin();
  ESPNOW_setup();
  //mac addr
  Serial. println(WiFi.macAddress());
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
  FFT_main();
}

void Uzurium_main(void){
  M5.update();

  
  //SW
  if(M5.BtnA.wasPressed()){
//    send_data(0,0,0,0,0);
    send_data(0,1,0,0,0);
    FinishMode();
    SetMode(MODE_A);
  }
  if(M5.BtnB.wasPressed()){
    send_data(0,1,0,0,0);
    FinishMode();
    SetMode(MODE_B);
  }
  if(M5.BtnC.wasPressed()){
    send_data(0,1,0,0,0);
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


  
}

