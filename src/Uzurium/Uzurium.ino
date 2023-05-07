

#include "./configuration.h"

DCMPWM motor = DCMPWM();
RINGLED led = RINGLED();

//モード定義
enum Mode{
  MODE_STOP,
  MODE_A,
  MODE_B,
  MODE_C,
  MODE_D
};
Mode Uzu_mode = MODE_STOP;

//Uzurium MODEをセットする
void Uzurium_SetMode(Mode mode){
  //実行中モードをFinishする
  if(MODE_A_CheckInit())MODE_A_Finish();
  if(MODE_STOP_CheckInit())MODE_STOP_Finish();

  Uzu_mode = mode;
  DUMP(mode);
}
//Uzurium 現在のMODEをCheckする
Mode Uzurium_CheckState(){
  return Uzu_mode;
}


unsigned long lastmillis; // タイムアウトのための時間変数

uint32_t startTime = 0;
uint32_t cycleTime = 0;

void setup() {

  //M5 INITIAL
  auto cfg =M5.config();
  cfg.internal_imu=false;
  cfg.internal_rtc =false;

  M5.begin(cfg);
  M5.In_I2C.release();
  Wire.begin(25,21);

 //SERIAL_INITIAL
  SERIAL_setup();
  
  //PIN MODE INITIAL
  PHOTO_setup();

  //INA219_INITIAL
  INA219_setup();

  //MOTOR INITIAL
  motor.setup(CHANNEL,MOTOR_PIN);

  //LED_INITIAL
  led.setup(LED_PIN);//led setup (led_num)

  //ESP-NOW INITIAL
  ESPNOW_setup();
  
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

  //初期モードにセット
  Uzurium_SetMode(MODE_STOP);
}

void Uzurium_Task(void *pvParameters){
  while(1){
    Uzurium_main();
    delay(1);
  }
}

void Uzurium_ClapModeChange(){
  if(FFT_CheckMagnitude() > 300){
    
    //MODE_Aで所定時間以上動作中ならストップ
    if(MODE_A_CheckInit() == true && MODE_A_CheckSpentTime() >= 1000){
      TRACE();
      Uzurium_SetMode(MODE_STOP);
    }
    //MODE_Aに入っていないならMODE_Aに入れる
    if(MODE_STOP_CheckInit() == true && MODE_STOP_CheckSpentTime() >=1000){
      TRACE();
      Uzurium_SetMode(MODE_A);
    }
  }
}

void Uzurium_main(){
  //モードチェック
  Mode mode = Uzurium_CheckState();

  //MODE_STOP
  if(mode == MODE_STOP){
    //MODE_A_Finish();
    MODE_STOP_main();
    led.pacifica();
    //led.fade(); // LED Fade OFF
    //SPEED_SetDuty(0); // SPEED set 0;
  }

  //MODE_A
  if(mode == MODE_A){
    MODE_A_main();
    int mag = FFT_CheckMagnitude();
    int hue = map(mag,0,256,0,256);
    led.fire2(4,hue);

    //StopDuty();
    //led.setbrightness(50);
  }
  //MODE_B
  if(mode == MODE_B){
    //FFT_main();
    //led.fire2(4,SPEED_CheckDuty());// mode ,duty
    //int brightness = map(SPEED_CheckDuty(),0,256,25,75);
    //led.setbrightness(brightness);
  }
  //MODE_C
  if(mode == MODE_C){
    
    //int brightness = map(SPEED_CheckDuty(),0,256,25,75);
    int mag = FFT_CheckMagnitude();
    int hue = map(mag,0,256,0,256);
    //Serial.println(hue);
    led.fire2(4,hue);
    
    //led.setbrightness(brightness);
  }
  //MODE_D
  if(mode == MODE_D){
    led.pacifica();
  }

  //シリアル入力チェック
  SERIAL_InputCheck();
  //
  Uzurium_ClapModeChange();
}

void loop() {

  M5.update();
  //Uzurium_main();
  FFT_main();
  //uint32_t deltaTime = millis() - cycleTime;
  //uint32_t spentTime = millis() - startTime;

  
}
