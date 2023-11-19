#include "./configuration.h"

DCMPWM motor = DCMPWM();
RINGLED led = RINGLED();
PHOTO photo = PHOTO();
SW switch1 = SW(SW_PIN,INPUT);
SW switch2 = SW(TEST_SW_PIN,INPUT_PULLUP);
//SW switch3 = SW(BTN_SW_PIN,INPUT);

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

// モードオブジェクトのポインタ
MODE *currentMode;


//Uzurium MODEをセットする
void Uzurium_SetMode(Mode mode){
  
  //実行中モードをFinishする
/*  if(MODE_A_CheckInit())MODE_A_Finish();
  if(MODE_B_CheckInit())MODE_B_Finish();
  if(MODE_C_CheckInit())MODE_C_Finish();
  if(MODE_D_CheckInit())MODE_D_Finish();*/
  //if(MODE_STOP_CheckInit())MODE_STOP_Finish();
  
  Uzu_mode = mode;
  DUMP(Uzu_mode);
}
//Uzurium 現在のMODEをCheckする
Mode Uzurium_CheckState(){
  return Uzu_mode;
}

unsigned long lastmillis; // タイムアウトのための時間変数


void setup() {
  //M5 INITIAL
  auto cfg =M5.config();
  cfg.internal_imu=false;
  cfg.internal_rtc =false;
  cfg.internal_spk =false;
  cfg.internal_mic =false;
  cfg.serial_baudrate = 115200;
  M5.begin(cfg);
  M5.In_I2C.release();
//  Wire.begin(25,21);
  //MOTOR INITIAL
  motor.setup(CHANNEL,MOTOR_PIN);

  //SERIAL_SetCheckIndex();

 //SERIAL_INITIAL
  //SERIAL_setup();

  //PIN MODE INITIAL
  //PHOTO_setup();
  photo.setup(PHOTO_PIN);
  //INA219_INITIAL
  //INA219_setup();

  //MOTOR SLEEP
//  pinMode(21, OUTPUT);
 // digitalWrite(21, HIGH);
  //pinMode(TEST_SW_PIN, INPUT_PULLUP);
  //LED_INITIAL
  led.setup(LED_PIN);//led setup (led_num)

  //ESP-NOW INITIAL
  ESPNOW_setup();
  
  //SW-INITIAL
  //SW_setup();
  //初期モードにセット
  currentMode = new StopMode(&photo,&motor,&led);
//  currentMode = new MODE();

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
  }
}

void checkSW(){
  //スイッチリード(モーメンタリ)
  int sw = switch1.check_m();
  //int sw3 = switch3.check_m();
  
  //スイッチリード(オルタネート)
  int sw2= switch2.check_a();
  //前回値からの変更有無
  bool sw2st = switch2.check_change();

  //TEST_SWが前回値から変更されていたらsw2の論理にあわせたモードへ変更
  if(sw2st){
      if(sw2==0){
        delete currentMode;
        currentMode = new TestMode(&photo,&motor,&led);
      }
      else{
        delete currentMode;
        currentMode = new StopMode(&photo,&motor,&led);
      }
  }

  if(sw2==1){//オルタネートSW = OFF
    if(sw==1){
      if(currentMode->name != "StopMode"){
        delete currentMode;
        currentMode = new StopMode(&photo,&motor,&led);
      }else{
        delete currentMode;
        currentMode = new ADinputMode(&photo,&motor,&led);
      }
    }
    if(sw==2){
        delete currentMode;
        currentMode = new FeedBackMode(&photo,&motor,&led);
    }
    if(sw==3){
        delete currentMode;
        currentMode = new TimerMode(&photo,&motor,&led);
    }
    if(M5.BtnA.wasPressed()){
        if(currentMode->name=="StopMode"){
          delete currentMode;
          currentMode = new ADinputMode(&photo,&motor,&led);
        }else 
        if(currentMode->name=="ADinputMode"){
          delete currentMode;
          currentMode = new FeedBackMode(&photo,&motor,&led);
        }else 
        if(currentMode->name=="FeedBackMode"){
          delete currentMode;
          currentMode = new StopMode(&photo,&motor,&led);
        }
    }
 
  }else{//オルタネートSW = ON
    if(sw==1){
      if(currentMode->name != "TestMode"){
        delete currentMode;
        currentMode = new TestMode(&photo,&motor,&led);
      }else{
        delete currentMode;
        currentMode = new ADInspectionMode(&photo,&motor,&led);
      }
    }
    if(sw==2){
        delete currentMode;
        currentMode = new PhotoReflectorInspectionMode(&photo,&motor,&led);
    }
    if(sw==3){
        delete currentMode;
        currentMode = new LedInspectionMode(&photo,&motor,&led);
      
    }
      if(M5.BtnA.wasPressed()){
        if(currentMode->name=="TestMode"){
          delete currentMode;
          currentMode = new ADInspectionMode(&photo,&motor,&led);
        }else 
        if(currentMode->name=="ADInspectionMode"){
          delete currentMode;
          currentMode = new PhotoReflectorInspectionMode(&photo,&motor,&led);
        }else 
        if(currentMode->name=="PhotoReflectorInspectionMode"){
          delete currentMode;
          currentMode = new TestMode(&photo,&motor,&led);
        }
    }
  }
}

void Uzurium_main(){

  
// 現在のモードのmainloop()を呼び出す
  currentMode->mainloop();
  currentMode->SetAdValue(FFT_CheckADvalue());

  //現在の実行モードが非アクティブになった場合はStopModeへ入れる
  if(currentMode->active==false){
    delete currentMode;
    currentMode = new StopMode(&photo,&motor,&led);
  }
  //所定モードの時のみ実行
  if(currentMode->name=="RemoteControlMode"){
    currentMode->SetParams(ESPNOW_CheckDuty(),ESPNOW_CheckHue(),ESPNOW_CheckBrightness());
  }
  if(currentMode->name=="ADInspectionMode"){
        Uzurium_Number = VR_CheckValue();
  }

}

void loop() {

  M5.update();
  //Uzurium_main();
  FFT_main();
  checkSW();
  //uint32_t deltaTime = millis() - cycleTime;
  //uint32_t spentTime = millis() - startTime;

  
}
