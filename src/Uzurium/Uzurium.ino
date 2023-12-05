#include "./configuration.h"
#if __has_include("Setting.h")
#include "./Setting.h"
#else
#define WIFI_SSID "your_ssid"
#define WIFI_PASS "your_password"
#endif
DCMPWM motor = DCMPWM();
RINGLED led = RINGLED();
PHOTO photo = PHOTO();
SW switch1 = SW(SW_PIN,INPUT);
SW switch2 = SW(TEST_SW_PIN,INPUT_PULLUP);

// モードオブジェクトのポインタ
MODE *currentMode;

// WiFiの接続処理
void initWiFi() {
  WiFi.mode(WIFI_STA);
  Serial.print("\nConnecting to " WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);     // WiFiに接続
  int i = 1;
  while (WiFi.status() != WL_CONNECTED) { // WiFiが繋がるまでループ
    if (i > 20) {                       // 10秒のタイムアウト
      Serial.println("\n========== WiFi connecting timeout! ==========\n");
      //ESP.restart();                    // リセット
      break;
    }
    delay(500);                         // 500ミリ秒待つ
    Serial.print(".");                  // ログに「.」を表示
    i++;
  }
  Serial.println("\nWiFi connected");   // ログに「WiFi connected」を表示
  Serial.print("IP address: ");         // ログにIPアドレスを表示
  Serial.println(WiFi.localIP());
}

void setup() {
  //M5 INITIAL

  M5.begin(true, false, true); 
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
  initWiFi();                           // WiFiの接続処理
  // NTPの初期化
  configTzTime("JST-9", "ntp.nict.jp"); 
  //timeInfo.tm_year + 1900 //timeInfo.tm_mon + 1 //timeInfo.tm_mday //timeInfo.tm_hour //timeInfo.tm_min //timeInfo.tm_sec

  //初期モードにセット
  if(switch2.check_a()==1){
    currentMode = new StopMode(&photo,&motor,&led);
  }
  else{
    currentMode = new TestMode(&photo,&motor,&led);
  }
  //Core0でタスク起動
  xTaskCreatePinnedToCore(
    Uzurium_Task        // タスク関数へのポインタ。無限ループで終了しないよう関数を指定します
    ,  "TaskMotion"   // タスクの説明用名前。重複しても動きますがデバッグ用途。最大16文字まで
    ,  4096  //スタックサイズ(Byte)
    ,  NULL //作成タスクのパラメータのポインタ
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest..  优先级，3 (configMAX_PRIORITIES - 1) 最高，0 最低。
    ,  NULL //作成タスクのHandleへのポインタ
    ,  0);  //利用するCPUコア(0-1)
}

void Uzurium_Task(void *pvParameters){
  while(1){
    Uzurium_main();
    delay(1);
  }
}

void Uzurium_Ntp(){
  struct tm timeInfo;
  getLocalTime(&timeInfo);
  if(timeInfo.tm_sec == 0 && currentMode->name == "StopMode"){
    delete currentMode;
    currentMode = new TimerMode(&photo,&motor,&led);
  }
}

void Uzurium_CheckSW(){
  //スイッチ(モーメンタリ)
  int sw = switch1.check_m();
  //スイッチ(オルタネート)
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
    // ダブル:3 ,長押し:2,短押し:1,押していない:0
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



    //M5AtomSWプッシュ時の動作
    if(M5.Btn.wasPressed()){
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
          currentMode = new TimerMode(&photo,&motor,&led);
        }else
        if(currentMode->name=="TimerMode"){
          delete currentMode;
          currentMode = new StopMode(&photo,&motor,&led);
        }

    }
 
  }else{//オルタネートSW = ON
    // ダブル:3 ,長押し:2,短押し:1,押していない:0
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

    if(M5.Btn.wasPressed()){
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
  
  //RemoteControlModeのときはESPNOW受信値をパラメータにセットする
  if(currentMode->name=="RemoteControlMode"){
    currentMode->SetParams(ESPNOW_CheckDuty(),ESPNOW_CheckHue(),ESPNOW_CheckBrightness());
  }
  //ADInspectionModeのときにUzurium_Numberを設定する
  if(currentMode->name=="ADInspectionMode"){
        Uzurium_Number =  map(FFT_CheckADvalue(),0,4095,1,18);
  }

}

void loop() {
  M5.update();
  //Analog pin読み取り
  FFT_main();
  //スイッチ状態チェック
  Uzurium_CheckSW();
  //Ntpチェック
  if(WiFi.status() == WL_CONNECTED){
    Uzurium_Ntp();
  }
}
