#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h> // only for esp_wifi_set_channel()
#include <M5Core2.h>

// Defines gestures
Gesture swipeRight("swipe right", 160, DIR_RIGHT, 30, true);
Gesture swipeDown("swipe down", 120, DIR_DOWN, 30, true);
Gesture swipeLeft("swipe left", 160, DIR_LEFT, 30, true);
Gesture swipeUp("swipe up", 120, DIR_UP, 30, true);

// Defines the buttons. Colors in format {bg, text, outline}
ButtonColors on_clrs = {RED, WHITE, WHITE};
ButtonColors off_clrs = {BLACK, WHITE, WHITE};
Button tl(0, 0, 0, 0, false ,"top-left", off_clrs, on_clrs, TL_DATUM);
Button bl(0, 0, 0, 0, false, "bottom-left", off_clrs, on_clrs, BL_DATUM);
Button tr(0, 0, 0, 0, false, "top-right", off_clrs, on_clrs, TR_DATUM);
Button br(0, 0, 0, 0, false, "bottom-right", off_clrs, on_clrs, BR_DATUM);

// For performance measurement (Single tap on bottom-right button)
uint32_t startTime;
uint32_t times = 0;



int duty = 0;
int mode = 0;

int SQmode =0; //SequenceMode
uint32_t SQstartTime;

esp_now_peer_info_t slave;
// 送信コールバック
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  // Serial.print("Last Packet Sent to: ");
  // Serial.println(macStr);
  // Serial.print("Last Packet Send Status: ");
  // Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  // 画面にも描画
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.print("Last Packet Sent to: \n  ");
  M5.Lcd.println(macStr);
  M5.Lcd.print("Last Packet Send Status: \n  ");
  M5.Lcd.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
// 受信コールバック
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.printf("Last Packet Recv from: %s\n", macStr);
  Serial.printf("Last Packet Recv Data(%d): ", data_len);
  for ( int i = 0 ; i < data_len ; i++ ) {
    Serial.print(data[i]);
    Serial.print(" ");
  }
  Serial.println("");
  // 画面にも描画
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.print("Last Packet Recv from: \n  ");
  M5.Lcd.println(macStr);
  M5.Lcd.printf("Last Packet Recv Data(%d): \n  ", data_len);
  for ( int i = 0 ; i < data_len ; i++ ) {
    M5.Lcd.print(data[i]);
    M5.Lcd.print(" ");
  }
  //if(data[0]==1)mode++;
  //if(mode>4)mode=0;
}

void setup() {
  M5.begin();
  M5.Lcd.fillScreen(BLACK);
  Serial.begin(115200);
//Button
  M5.Buttons.addHandler(doRotation, E_GESTURE);
  M5.Buttons.addHandler(toggleColor, E_DBLTAP);
  M5.Buttons.addHandler(eventDisplay, E_ALL - E_MOVE);
  br.addHandler(showPerformance, E_TAP);
  br.repeatDelay = 1000;
  doButtons();
  startTime = millis();
// ESP-NOW初期化
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow Init Success");
    M5.Lcd.print("ESPNow Init Success\n");
  } else {
    Serial.println("ESPNow Init Failed");
    M5.Lcd.print("ESPNow Init Failed\n");
    ESP.restart();
  }
  // マルチキャスト用Slave登録
  memset(&slave, 0, sizeof(slave));
  for (int i = 0; i < 6; ++i) {
    slave.peer_addr[i] = (uint8_t)0xff;
  }
  esp_err_t addStatus = esp_now_add_peer(&slave);
  if (addStatus == ESP_OK) {
    // Pair success
    Serial.println("Pair success");
  }
  // ESP-NOWコールバック登録
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);

  //VL53
  Wire.begin(32, 33);  // join i2c bus (address optional for master)
}


void send_data(int num,int mode,int duty){
  uint8_t data[4] = {num,mode,0,duty};//num,mode,colormode,duty
    esp_err_t result = esp_now_send(slave.peer_addr, data, sizeof(data));
    Serial.print("Send Status: ");
    if (result == ESP_OK) {
      Serial.println("Success");
    } else if (result == ESP_ERR_ESPNOW_NOT_INIT) {
      Serial.println("ESPNOW not Init.");
    } else if (result == ESP_ERR_ESPNOW_ARG) {
      Serial.println("Invalid Argument");
    } else if (result == ESP_ERR_ESPNOW_INTERNAL) {
      Serial.println("Internal Error");
    } else if (result == ESP_ERR_ESPNOW_NO_MEM) {
      Serial.println("ESP_ERR_ESPNOW_NO_MEM");
    } else if (result == ESP_ERR_ESPNOW_NOT_FOUND) {
      Serial.println("Peer not found.");
    } else {
      Serial.println("Not sure what happened");
    }
    delay(30);

}

void StartSQ(int mode){
  SQmode = mode;
  SQstartTime = millis();
}
void EndSQ(){
  SQmode = 0 ;  
}
int checkSQ(){
  return SQmode;
}
void Sequence1(){
  int span =3000;
  int duty =120;
  int duty2 =150;
  int duty3 =200;
  int time = millis() - SQstartTime;
  if(time<span*1){
    send_data(1,2,duty);
    //Serial.print("1");
  }else if(span*1<=time && time<span*2){
    send_data(2,2,duty);
    //Serial.print("2");
  }
  else if(span*2<=time && time<span*3){
    send_data(3,2,duty);
    //Serial.print("3");
  }
  else if(span*3<=time && time<span*4){
    send_data(4,2,duty);
    //Serial.print("4");
  }
    else if(span*4<=time && time<span*5){
    send_data(5,2,duty);
  }
    else if(span*5<=time && time<span*6){
    send_data(1,2,duty2);
    send_data(2,2,duty2);
    send_data(3,2,duty2);
    send_data(4,2,duty2);
    send_data(5,2,duty2);
  }  else if(span*6<=time && time<span*7){
    send_data(1,2,duty3);
    send_data(2,2,duty3);
    send_data(3,2,duty3);
    send_data(4,2,duty3);
    send_data(5,2,duty3);
  }else if(span*7<=time){
    send_data(1,0,0);
    send_data(2,0,0);
    send_data(3,0,0);
    send_data(4,0,0);
    send_data(5,0,0);
    EndSQ();
    Serial.println("END_SQ");
  }

}
void Sequence2(){
  int time = millis() - SQstartTime;
  int span = 5000;
  int duty =0;
  if(time<span*1){
    send_data(1,0,duty);
    send_data(2,0,duty);
    send_data(3,0,duty);
    send_data(4,0,duty);
    send_data(5,0,duty);
  }else if(span*1<=time){
    EndSQ();
  }
}


void loop() {
  M5.update();
  //touch panel用
  times++;
  //距離センサの値をMOTOR DUTYへ変換
  duty = 256 - check_dep(0,256);
  //DUTYを元に色を決定
  int lv1 =100;
  int lv2 =200;
   if(duty <= lv1){
    mode = 1;
  }else if(lv1 < duty && duty <lv2){
    mode = 2;
  }else if(lv2 <= duty){
    mode = 3;    
  }else{
    mode =0;    
  }
  if(checkSQ()==0){
    send_data(1,mode,duty);
    send_data(2,mode,duty);
    send_data(3,mode,duty);
    send_data(4,mode,duty);
    send_data(5,mode,duty);
  }
  if(checkSQ()==1)Sequence1();
  if(checkSQ()==2)Sequence2();


  Serial.print("duty=");
  Serial.println(duty);

  if(M5.BtnA.wasPressed()){
    if(checkSQ()==0)StartSQ(1);
    Serial.println("START_SQ1");
  }
  if(M5.BtnB.wasPressed()){
    if(checkSQ()==0)StartSQ(2);
    Serial.println("START_SQ2");
  }
  //シリアルからモード設定
  if (Serial.available())
  {
    int rec=Serial.parseInt();
    Serial.print("rec=");
    Serial.println(rec);

    if(256>=rec && rec>= 1){//serial print改行コード0は無視
      duty = rec ;
      send_data(1,mode,duty);
      }
    Serial.print("duty=");
    Serial.println(duty);
    
  }else{
    //mode=4;//通信途切れたらオフ
  }
  delay(30);
  
}
// Positions the buttons and draws them. (Only because height and width
// change. If we were only switching between normal and upside-down we
// would only need the Buttons.draw() here.)
void doButtons() {
  int16_t hw = M5.Lcd.width() / 2;
  int16_t hh = M5.Lcd.height() / 2;
  tl.set(0, 0, hw - 5, hh - 5);
  bl.set(0, hh + 5, hw - 5, hh - 5);
  tr.set(hw + 5, 0, hw - 5, hh - 5);
  br.set(hw + 5, hh + 5, hw - 5, hh - 5);
  M5.Buttons.draw();
}

void doRotation(Event& e) {
  // Gestures and Buttons have an instanceIndex() that starts at zero
  // so by defining the gestures in the right order I can use that as
  // the input for M5.Lcd.setRotation.
  uint8_t new_rotation = e.gesture->instanceIndex();
  if (new_rotation != M5.Lcd.rotation) {
    M5.Lcd.clearDisplay();
    M5.Lcd.setRotation(new_rotation);
    doButtons();
  }
}

void toggleColor(Event& e) {
  // Just so we can type "b." instead of "e.button->"
  Button& b = *e.button;

  if (b != M5.background) {
    // Toggles the button color between black and blue
    b.off.bg = (b.off.bg == BLACK) ? BLUE : BLACK;
    b.draw();
  }
}

void showPerformance(Event& e) {
  Serial.printf("%d in %d ms, average M5.update() took %.2f microseconds\n",
   times, millis() - startTime, (float)((millis() - startTime) * 1000) / times);
  startTime = millis();
  times = 0;
}

void eventDisplay(Event& e) {
  Serial.printf("%-12s finger%d  %-18s (%3d, %3d) --> (%3d, %3d)   ",
                e.typeName(), e.finger, e.objName(), e.from.x, e.from.y,
                e.to.x, e.to.y);
  Serial.printf("( dir %d deg, dist %d, %d ms )\n", e.direction(),
                e.distance(), e.duration);
}
