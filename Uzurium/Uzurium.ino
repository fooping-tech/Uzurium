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

  
  //PIN MODE INITIAL
  PHOTO_setup();

  //INA219_INITIAL
  INA219_setup();

  //MOTOR INITIAL
  motor.setup(CHANNEL,MOTOR_PIN);

  //LED_INITIAL
  led.setup(LED_PIN);//led setup (led_num)

  //SERIAL_INITIAL
  SERIAL_setup();

  //ESP-NOW INITIAL
  ESPNOW_setup();
}



void loop() {

  M5.update();
  uint32_t deltaTime = millis() - cycleTime;
  uint32_t spentTime = millis() - startTime;
/*
  //定期実行タスク
  if(CheckSQ() && deltaTime >=50){
    //RPMを計測する
    PHOTO_CalcNowRPM();
    //シリアル通信のデータを書き出す
    SERIAL_SetSerialData();
    //エッジがしばらく来ない場合にRPM初期化
    PHOTO_CheckTimeout();
    //PID制御によりDUTYを計算する
    SPEED_ClacDuty(deltaTime);
    //脱調判定
    PHOTO_check();
    //TargetRPM設定
    if(spentTime>0)PHOTO_Set_TargetRPM(0);
    if(spentTime>200)PHOTO_Set_TargetRPM(1000);
    if(spentTime>1000)PHOTO_Set_TargetRPM(PHOTO_Check_outRPM());
    //cycleTimeリセット
    cycleTime = millis();
  }
*/
 Mode mode = Uzurium_GetState();

  //MODE_STOP
  if(mode == MODE_STOP){
    led.fade(); // LED Fade OFF
    SPEED_SetDuty(0); // SPEED set 0;
  }

  //MODE_A
  if(mode == MODE_A){
    led.pacifica();
    //StopDuty();
    //led.setbrightness(50);
  }
  //MODE_B
  if(mode == MODE_B){
    led.fire2(4,SPEED_CheckDuty());// mode ,duty
    int brightness = map(SPEED_CheckDuty(),0,256,25,75);
    //led.setbrightness(brightness);
  }
  //MODE_C
  if(mode == MODE_C){
    led.fire2(0,SPEED_CheckDuty());
    int brightness = map(SPEED_CheckDuty(),0,256,25,75);
    //led.setbrightness(brightness);
  }
  //MODE_D
  if(mode == MODE_D){
    MODE_A_main();
    /*
    if(!CheckSQ()){
      SPEED_StartDuty(0);
    }
    */
  }
  
  motor.move(SPEED_CheckDuty());
  
  SERIAL_InputCheck();

}
