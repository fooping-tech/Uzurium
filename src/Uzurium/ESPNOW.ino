esp_now_peer_info_t slave;

// MACアドレスを設定
uint8_t specificMacAddress[] = {0x78, 0x21, 0x84, 0x95, 0x80, 0x10};
//uint8_t specificMacAddress[] = {0x08, 0x3A, 0xF2, 0x44, 0xF5, 0x90};

int ESPNOW_duty =0;
int ESPNOW_hue =0;
int ESPNOW_brightness=25;

int ESPNOW_CheckDuty(){
  DUMP(ESPNOW_duty);
  return ESPNOW_duty;
}
int ESPNOW_CheckHue(){
  DUMP(ESPNOW_hue);
  return ESPNOW_hue;
}
int ESPNOW_CheckBrightness(){
  return ESPNOW_brightness;
}

void ESPNOW_setup(){
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    if (esp_now_init() == ESP_OK) {
      Serial.println("ESPNOW setup was completed.");
      //M5.Lcd.print("ESPNow Init Success\n");
    } else {
      Serial.println("ESPNow Init Failed");
      //M5.Lcd.print("ESPNow Init Failed\n");
      ESP.restart();
    }
 
    slave.peer_addr[0] = (uint8_t)0x78;
    slave.peer_addr[1] = (uint8_t)0x21;
    slave.peer_addr[2] = (uint8_t)0x84;
    slave.peer_addr[3] = (uint8_t)0x95;
    slave.peer_addr[4] = (uint8_t)0x80;
    slave.peer_addr[5] = (uint8_t)0x10;
    // マルチキャスト用Slave登録
    memset(&slave, 0, sizeof(slave));
  // 特定のMACアドレス（specificMacAddress）をpeerInfoにコピー
  memcpy(slave.peer_addr, specificMacAddress, sizeof(specificMacAddress));
  //for (int i = 0; i < 6; ++i) {
  //  slave.peer_addr[i] = (uint8_t)0xff;
  //}
    esp_err_t addStatus = esp_now_add_peer(&slave);
    if (addStatus == ESP_OK) {
      // Pair success
      Serial.println("ESPNOW Pair success");
    }
    // ESP-NOWコールバック登録
    esp_now_register_send_cb(OnDataSent);
    esp_now_register_recv_cb(OnDataRecv);
}

// 送信コールバック
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Last Packet Sent to: ");
  Serial.println(macStr);
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  }

// 受信コールバック
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  //Serial.printf("Last Packet Recv from: %s\n", macStr);
  //Serial.printf("Last Packet Recv Data(%d): ", data_len);

  bool flag =true;
  for(int i=0;i<5;i++){
    if(mac_addr[i] != specificMacAddress[i])flag = false;
  }
  DUMP(flag);
  if(flag){
    int mode=data[0];
    int value1=data[1];
    int value2=data[2];
    int value3=data[3];
    int value4=data[4];
  /*
    for ( int i = 0 ; i < data_len ; i++ ) {
      Serial.print(data[i]);
      Serial.print(" ");
    }
    Serial.println("");
  */
  //mode=0のとき
    if(mode==0){
      switch(value1){
          case 0:
            Uzurium_SetMode(MODE_STOP);
            break;
          case 4:
            Uzurium_SetMode(MODE_A);
            break;
          case 2:
            Uzurium_SetMode(MODE_B);
            break;
          case 3:
            Uzurium_SetMode(MODE_C);
            break;
          case 1:
            Uzurium_SetMode(MODE_D);
            break;
        }
    }

    //mode=1のとき
    if(mode==1 && value1==Uzurium_Number){
      ESPNOW_duty = value2;
      ESPNOW_hue = value3;
      ESPNOW_brightness = value4;
    }
  }
}
