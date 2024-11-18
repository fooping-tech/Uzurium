esp_now_peer_info_t slave;

// MACアドレスを設定
//uint8_t specificMacAddress[] = {0x78, 0x21, 0x84, 0x95, 0x80, 0x10};
uint8_t specificMacAddress[] = {0x10, 0x06, 0x1C, 0x0A, 0xBF, 0xF4};
//uint8_t specificMacAddress[] = {0x08, 0x3A, 0xF2, 0x44, 0xF5, 0x90};

int ESPNOW_duty =0;
int ESPNOW_hue =0;
int ESPNOW_brightness=25;

int ESPNOW_CheckDuty(){
//  DUMP(ESPNOW_duty);
  return ESPNOW_duty;
}
int ESPNOW_CheckHue(){
//  DUMP(ESPNOW_hue);
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
    slave.peer_addr[0] = (uint8_t)0x10;
    slave.peer_addr[1] = (uint8_t)0x06;
    slave.peer_addr[2] = (uint8_t)0x1C;
    slave.peer_addr[3] = (uint8_t)0x0A;
    slave.peer_addr[4] = (uint8_t)0xBF;
    slave.peer_addr[5] = (uint8_t)0xF4;
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
  int id=data[0];
  if(flag && id==5){
    ESPNOW_duty = data[Uzurium_Number*3-2];
    ESPNOW_hue = data[Uzurium_Number*3-1];
    ESPNOW_brightness  = data[Uzurium_Number*3];
  }
}

