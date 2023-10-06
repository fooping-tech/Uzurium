int cur_value = 0;
int last_value = 0;

unsigned long lastDebounceTime = 0; // 最後にボタンが押された時間
unsigned long debounceDelay = 50;   // デバウンス用の遅延時間（ミリ秒）
bool buttonLongPressed = false; // ボタンが長押し状態かどうかを示すフラグ
unsigned long longPressDuration = 1000; // 長押しとみなす時間（ミリ秒）

int lastClickTime = 0; // 最後のクリック時間
int doubleClickDelay = 300; // ダブルクリックとみなす時間間隔（ミリ秒）

  void SW_setup(){
   pinMode(SW_PIN, INPUT);
   Serial.println("SW setup was completed.");
  }

  bool SW_check(){
  
    cur_value = digitalRead(SW_PIN);
  
    if(cur_value != last_value){  //前回値と今回値が異なり
      last_value = cur_value;       //前回値を更新
      if(cur_value==0){               //今回0のとき(0がSW=0N)
        return true;
      }else return false;
    }else{
          last_value = cur_value;       //前回値を更新
          return false;
    }


  }

//長押し:2,短押し:1,押していない:0
int SW_check2(){
  cur_value = digitalRead(SW_PIN);

  if(cur_value != last_value){  // 前回値と今回値が異なり
    last_value = cur_value;       // 前回値を更新
    if(cur_value == LOW){         // 今回がボタンが押されたとき
      // ボタンが長押し状態かどうかを判定
      if ((millis() - lastDebounceTime) > longPressDuration) {
        buttonLongPressed = true;
        DUMP("LONG");
        return 2;
      }
      DUMP("BUTTON");
      return 1;

    }
    else { // 今回がボタンが離されたとき
      lastDebounceTime = millis(); // ボタンが離された時間を記録
      buttonLongPressed = false; // ボタンの長押し状態をリセット
    }
  }

  last_value = cur_value; // 前回値を更新
  //DUMP("FALSE");

  return 0;
}


//ダブル:3 ,長押し:2,短押し:1,押していない:0
int SW_check3(){
  cur_value = digitalRead(SW_PIN);

  if(cur_value != last_value){  // 前回値と今回値が異なり
    last_value = cur_value;       // 前回値を更新
    if(cur_value == LOW){         // 今回がボタンが押されたとき
      // ボタンが長押し状態かどうかを判定
      if ((millis() - lastDebounceTime) > longPressDuration) {
        buttonLongPressed = true;
        DUMP("LONG");
        return 2;
      }else{
        // シングルクリックを検出
        int currentTime = millis();
        if (currentTime - lastClickTime < doubleClickDelay) {
          lastClickTime = 0; // クリック間隔をリセット
          DUMP("DOUBLE");
          return 3; // ダブルクリックを示す値
        } else {
          lastClickTime = currentTime;
          DUMP("SINGLE");
          return 1; // シングルクリックを示す値
        }
      }
    }
    else { // 今回がボタンが離されたとき
      lastDebounceTime = millis(); // ボタンが離された時間を記録
      buttonLongPressed = false; // ボタンの長押し状態をリセット
    }
  }

  last_value = cur_value; // 前回値を更新
  //DUMP("FALSE");

  return 0;
}