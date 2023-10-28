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
int Uzurium_Number = 0;
//Uzurium MODEをセットする
void Uzurium_SetMode(Mode mode){
  
  //実行中モードをFinishする
  if(MODE_A_CheckInit())MODE_A_Finish();
  if(MODE_B_CheckInit())MODE_B_Finish();
  if(MODE_C_CheckInit())MODE_C_Finish();
  if(MODE_D_CheckInit())MODE_D_Finish();
  //if(MODE_STOP_CheckInit())MODE_STOP_Finish();
  
  Uzu_mode = mode;
  DUMP(Uzu_mode);
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
  //Wire.begin(25,21);

  SERIAL_SetCheckIndex();

 //SERIAL_INITIAL
  SERIAL_setup();
  
  //PIN MODE INITIAL
  PHOTO_setup();

  //INA219_INITIAL
  //INA219_setup();

  //MOTOR INITIAL
  motor.setup(CHANNEL,MOTOR_PIN);
  //MOTOR SLEEP
  pinMode(21, OUTPUT);
  digitalWrite(21, HIGH);
  //LED_INITIAL
  led.setup(LED_PIN);//led setup (led_num)

  //ESP-NOW INITIAL
  ESPNOW_setup();
  
  //SW-INITIAL
  SW_setup();

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
  BUZZER_On(0);
  led.setbrightness(LedBrightness);

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

  //SW
  int sw = SW_check3();
  if(sw==2){
    
    bool mode_active=false;
    if(MODE_A_CheckInit() == true && MODE_A_CheckSpentTime() >= 100)mode_active=true;
    if(MODE_B_CheckInit() == true && MODE_B_CheckSpentTime() >= 100)mode_active=true;
    if(MODE_C_CheckInit() == true && MODE_C_CheckSpentTime() >= 100)mode_active=true;
    if(MODE_D_CheckInit() == true && MODE_D_CheckSpentTime() >= 100)mode_active=true;
    if(mode_active){
      Uzurium_SetMode(MODE_STOP);
      BUZZER_On(1);
      TRACE();
    }else{
      Uzurium_SetMode(MODE_A);
      BUZZER_On(0);

      TRACE();
    }
  }else if(sw==1){
    bool mode_active=false;

    if(MODE_A_CheckInit() == true && MODE_A_CheckSpentTime() >= 100)mode_active=true;
    if(MODE_B_CheckInit() == true && MODE_B_CheckSpentTime() >= 100)mode_active=true;
    if(MODE_C_CheckInit() == true && MODE_C_CheckSpentTime() >= 100)mode_active=true;
    if(MODE_D_CheckInit() == true && MODE_D_CheckSpentTime() >= 100)mode_active=true;
    
    if(mode_active){
      Uzurium_SetMode(MODE_STOP);
      BUZZER_On(1);
    }else{
      DUMP("MODE_B");
      Uzurium_SetMode(MODE_B);
      BUZZER_On(0);
      
    }
  }else if(sw==3){
      DUMP("MODE_C");
      Uzurium_SetMode(MODE_C);
  }


    //モードチェック
  Mode mode = Uzurium_CheckState();
  //MODE_STOP
  if(mode == MODE_STOP){
    //MODE_A_Finish();
    MODE_STOP_main();
    //led.fade(); // LED Fade OFF
    //SPEED_SetDuty(0); // SPEED set 0;
  }

  //MODE_A
  if(mode == MODE_A){
    MODE_A_main();
    int mag = FFT_CheckMagnitude();
    int hue = map(mag,0,256,0,256);
    //led.fire2(4,hue);
    

    //StopDuty();
    //led.setbrightness(50);
  }
  //MODE_B
  if(mode == MODE_B){
    MODE_B_main();
    int mag = FFT_CheckMagnitude();
    int hue = map(mag,0,256,0,256);
//    led.fire2(4,hue);
    

    //FFT_main();
    //led.fire2(4,SPEED_CheckDuty());// mode ,duty
    //int brightness = map(SPEED_CheckDuty(),0,256,25,75);
    //led.setbrightness(brightness);
  }
  //MODE_C
  if(mode == MODE_C){
    MODE_C_main();
  }
  //MODE_D
  if(mode == MODE_D){
    MODE_D_main();    
  }

  //シリアル入力チェック
  SERIAL_InputCheck();
  //
  //Uzurium_ClapModeChange();
  //buzzer
  BUZZER_main();
  VR_main();
}

void loop() {

  M5.update();
  //Uzurium_main();
  FFT_main();
  //uint32_t deltaTime = millis() - cycleTime;
  //uint32_t spentTime = millis() - startTime;

  
}
