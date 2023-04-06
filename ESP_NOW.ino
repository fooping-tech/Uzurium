esp_now_peer_info_t slave;


void ESPNOW_setup(){
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    if (esp_now_init() == ESP_OK) {
      Serial.println("ESPNow Init Success");
      //M5.Lcd.print("ESPNow Init Success\n");
    } else {
      Serial.println("ESPNow Init Failed");
      //M5.Lcd.print("ESPNow Init Failed\n");
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
  Serial.printf("Last Packet Recv from: %s\n", macStr);
  Serial.printf("Last Packet Recv Data(%d): ", data_len);
  for ( int i = 0 ; i < data_len ; i++ ) {
    Serial.print(data[i]);
    Serial.print(" ");
  }
  Serial.println("");
  //NUMBER宛の信号だけ読み取る
  if(data[0]==NUMBER){
    switch(data[1]){
        case 1:
          Uzurium_SetMode(MODE_STOP);
          break;
        case 2:
          Uzurium_SetMode(MODE_A);
          break;
        case 3:
          Uzurium_SetMode(MODE_B);
          break;
        case 4:
          Uzurium_SetMode(MODE_C);
          break;
        case 5:
          Uzurium_SetMode(MODE_D);
          break;
    }
//    mode = data[1];
//    color = data[2];
    SPEED_SetDuty(data[3]);
//    duty = data[3];
  }
}