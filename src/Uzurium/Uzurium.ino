#include "./configuration.h"

DCMPWM motor = DCMPWM();
RINGLED led = RINGLED();
PHOTO photo = PHOTO();
SW switch1 = SW(SW_PIN,INPUT);
SW switch2 = SW(TEST_SW_PIN,INPUT_PULLUP);

int Uzurium_Number = 0;

// モードオブジェクトのポインタ
MODE *currentMode;

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
  //MOTOR INITIAL
  motor.setup(CHANNEL,MOTOR_PIN);
  photo.setup(PHOTO_PIN);

  //MOTOR SLEEP
  // pinMode(21, OUTPUT);
  // digitalWrite(21, HIGH);
  //LED_INITIAL
  led.setup(LED_PIN);//led setup (led_num)
  //ESP-NOW INITIAL
  ESPNOW_setup();
  //初期モードにセット
  currentMode = new StopMode(&photo,&motor,&led);
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
  //LEDの明るさを設定
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

void Uzurium_CheckSW(){
  //スイッチリード(モーメンタリ)
  int sw = switch1.check_m();
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
        Uzurium_Number =  map(FFT_CheckADvalue(),0,4095,1,18);
  }

}

void loop() {
  M5.update();
  FFT_main();
  Uzurium_CheckSW();
}
