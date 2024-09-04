int SERIAL_duty = 0;
int SERIAL_hue = 0;
int SERIAL_brightness = 25;

int SERIAL_CheckDuty() {
  //  DUMP(SERIAL_duty);
  return SERIAL_duty;
}
int SERIAL_CheckHue() {
  //  DUMP(SERIAL_hue);
  return SERIAL_hue;
}
int SERIAL_CheckBrightness() {
  return SERIAL_brightness;
}

void SERIAL_setup() {
  //SERIAL INITIAL
  Serial.begin(9600);  //Serial begin
  delay(50);             //delay 50ms.  延迟50ms
  Serial.println("SERIAL setup was completed.");
}


/**
 * 指定された文字列から指定された位置の単語を取得する関数。
 *
 * @param inputString 取得対象の文字列
 * @param wordIndex 取得する単語の位置
 * @return 指定された位置の単語。見つからない場合は空の文字列を返します。
 */
String SERIAL_getWordFromIndex(String inputString, int wordIndex) {
  // 単語の個数をカウントする変数
  int wordCount = 0;
  // 単語の開始位置
  int wordStartIndex = 0;
  // 単語の終了位置
  int wordEndIndex = -1;
  // 文字列の最大インデックス
  int maxIndex = inputString.length() - 1;

  // 文字列をスキャンして単語を探索
  for (int i = 0; i <= maxIndex && wordCount <= wordIndex; i++) {
    // 現在の文字がスペースか、最後の文字である場合、単語が見つかったと判断
    if (inputString.charAt(i) == ' ' || i == maxIndex) {
      // 単語の個数を増加させる
      wordCount++;
      // 単語の開始位置を設定
      wordStartIndex = wordEndIndex + 1;

      // 単語の終了位置を設定。最後の文字の場合は終了位置を文字列の末尾に設定
      if (i == maxIndex) {
        wordEndIndex = i + 1;
      } else {
        wordEndIndex = i;
      }
    }
  }

  // 指定された位置の単語を返す。見つからない場合は空の文字列を返す
  if (wordCount > wordIndex) {
    return inputString.substring(wordStartIndex, wordEndIndex);
  } else {
    return "";
  }
}


void SERIAL_InputCheck() {
  //シリアルが利用可能か確認
  if (Serial.available()) {
    // シリアル読み取り
    String input = Serial.readStringUntil('\n');
    // 大文字へ
    input.toUpperCase();
    //コマンドを取得
    String command = SERIAL_getWordFromIndex(input, 0);

    if (command == F("MODE")) {
      int value1 = SERIAL_getWordFromIndex(input, 1).toInt();//mode_number 0:stop,1:SerialControlMode
      //value2 = SERIAL_getWordFromIndex(input, 2);
      //value3 = SERIAL_getWordFromIndex(input, 3);
      //value4 = SERIAL_getWordFromIndex(input, 4);
      switch(value1){
          case 0:
            delete currentMode;
            currentMode = new StopMode(&photo,&motor,&led);
            break;
          case 1:
            delete currentMode;
            currentMode = new SerialControlMode(&photo,&motor,&led);
            break;
      }
      Serial.println("Mode_OK");
    }

    if (command == F("SET")) {
      int value1 = SERIAL_getWordFromIndex(input, 1).toInt();//duty
      int value2 = SERIAL_getWordFromIndex(input, 2).toInt();//hue
      int value3 = SERIAL_getWordFromIndex(input, 3).toInt();//brightness

      //モータ保護のためDUTY制限
      if(value1 > 200)value1 = 200;
      if(value1 < 0)value1 = 0;

      //LED熱故障対策のため光量制限
      if(value3 > 200)value3 = 200;
      if(value3 < 0)value3 = 0;

        SERIAL_duty = value1;
        SERIAL_hue = value2;
        SERIAL_brightness = value3;
      Serial.println("Set_OK");
    }

    if (command == F("DUTY")) {
      int value1 = SERIAL_getWordFromIndex(input, 1).toInt();//duty
      //モータ保護のためDUTY制限
      if(value1 > 200)value1 = 200;
      if(value1 < 0)value1 = 0;

        SERIAL_duty = value1;
      Serial.println("duty_OK");
    }
    if (command == F("HUE")) {
      int value1 = SERIAL_getWordFromIndex(input, 1).toInt();//hue
        SERIAL_hue = value1;
      Serial.println("hue_OK");
    }
    if (command == F("BRIGHT")) {
      int value1 = SERIAL_getWordFromIndex(input, 1).toInt();//bright
        SERIAL_brightness = value1;
      Serial.println("brightness_OK");
    }


  } else {
      //Serial通信切断時の処理
  }
}