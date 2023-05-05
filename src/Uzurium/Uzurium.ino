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
  Uzu_mode = mode;
  //Serial.print("SetState: ");
  //Serial.println(mode);
}
//Uzurium 現在のMODEをゲットする
Mode Uzurium_GetState(){
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
  Uzurium_SetMode(MODE_D);
}

void Uzurium_Task(void *pvParameters){
  while(1){
    Uzurium_main();
    delay(1);
  }
}
void Uzurium_main(){
  Mode mode = Uzurium_GetState();

    //MODE_STOP
    if(mode == MODE_STOP){
      led.fade(); // LED Fade OFF
      SPEED_SetDuty(0); // SPEED set 0;
    }

    //MODE_A
    if(mode == MODE_A){
      MODE_A_main();
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
      led.fire2(0,SPEED_CheckDuty());
      //int brightness = map(SPEED_CheckDuty(),0,256,25,75);
      //led.setbrightness(brightness);
    }
    //MODE_D
    if(mode == MODE_D){
      led.pacifica();
    }

    //シリアル入力チェック
    SERIAL_InputCheck();

}

void loop() {

  M5.update();
  FFT_main();
  //uint32_t deltaTime = millis() - cycleTime;
  //uint32_t spentTime = millis() - startTime;

  
}
