#define ESPNOW_SEND_WAIT_TIME 40//ESP-NOW送信後の待ち時間(短いほうがレスポンス高いが送信負荷も高い)

#define NUM_PEERS 1 // 登録するMACアドレスの数

esp_now_peer_info_t slaves[NUM_PEERS];
//esp_now_peer_info_t slave;

// MACアドレスの配列
//esp_now_sendの処理が重いため全slaveに対して一斉送信
uint8_t slave_addrs[NUM_PEERS][6] = {
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
};

/*
//個別のslaveアドレスを登録
uint8_t slave_addrs[NUM_PEERS][6] = {
    {0x00, 0x11, 0x22, 0x33, 0x44, 0x55},
    {0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB},
    {0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11},
    {0x22, 0x33, 0x44, 0x55, 0x66, 0x77},
    {0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD}
};
*/

// デバイス自身のMACアドレスを取得して表示
void ESPNOW_checkAddress(){
    
    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    Serial.print("ESP-NOW MAC address: ");
    for (int i = 0; i < 6; i++) {
      if (i > 0) Serial.print(":");
      if (mac[i] < 16) Serial.print("0");  // 1桁の場合にゼロを追加
      Serial.print(mac[i], HEX);
    }
    Serial.println();
}

void ESPNOW_setup(){

    // ESP-NOW初期化
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
  //macAddressをSerialに表示
  ESPNOW_checkAddress();
  // 各MACアドレスを登録
  for (int i = 0; i < NUM_PEERS; ++i) {
      memcpy(slaves[i].peer_addr, slave_addrs[i], 6);
      esp_err_t addStatus = esp_now_add_peer(&slaves[i]);
      if (addStatus == ESP_OK) {
          Serial.printf("Pair success with peer %d\n", i);
      } else {
          Serial.printf("Failed to pair with peer %d\n", i);
      }
  }
  /*
  // マルチキャスト用Slave登録(全アドレスへ一斉送信)
  memset(&slave, 0, sizeof(slave));
  for (int i = 0; i < 6; ++i) {
    slave.peer_addr[i] = (uint8_t)0xff;
  }
  esp_err_t addStatus = esp_now_add_peer(&slave);
  if (addStatus == ESP_OK) {
    // Pair success
    Serial.println("Pair success");
  }
  */
 
  // ESP-NOWコールバック登録
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);

}
// 送信コールバック
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  //Serial.print("Last Packet Sent to: ");
  //Serial.println(macStr);
  //Serial.print("Last Packet Send Status: ");
  //Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
 
}
 
// 受信コールバック
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  //Serial.printf("Last Packet Recv from: %s\n", macStr);
  //Serial.printf("Last Packet Recv Data(%d): ", data_len);
  //M5.dis.drawpix(0, dispColor(255,0,0)); //LED（指定色）
  for ( int i = 0 ; i < data_len ; i++ ) {
  //  Serial.print(data[i]);
  //  Serial.print(" ");

  }
  //Serial.println("");
}

// データ送信関数

void send_data(int mode, int value1, int value2, int value3, int value4) {
    uint8_t data[5] = {mode, value1, value2, value3, value4};

    // すべての登録されたMACアドレスにデータを送信
    for (int i = 0; i < NUM_PEERS; ++i) {
        esp_err_t result = esp_now_send(slaves[i].peer_addr, data, sizeof(data));
        if (result == ESP_OK) {
            Serial.printf("Data sent to peer %d\n", i);
        } else {
            Serial.printf("Failed to send data to peer %d\n", i);
        }
        delay(ESPNOW_SEND_WAIT_TIME);
    }
}
/*
void send_data(int mode, int value1, int value2, int value3, int value4, int peerIndex) {
    if (peerIndex < 0 || peerIndex >= NUM_PEERS) {
        Serial.println("Invalid peer index");
        return;
    }
    uint8_t data[5] = {mode, value1, value2, value3, value4};
    esp_err_t result = esp_now_send(slaves[peerIndex].peer_addr, data, sizeof(data));
    delay(ESPNOW_SEND_WAIT_TIME);
}
*/
/*
void send_data(int mode,int value1,int value2,int value3,int value4){
    uint8_t data[5] = {mode,value1,value2,value3,value4};

    esp_err_t result = esp_now_send(slave.peer_addr, data, sizeof(data));

    delay(ESPNOW_SEND_WAIT_TIME);

}*/