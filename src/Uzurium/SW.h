#ifndef SW_H
#define SW_H

#include <M5Unified.h>

class SW {


public:
  const int SW_PIN; // ピン番号は定数としてクラスに含める
  //コンストラクタ：ピン番号、入力モード(INPUT,INPUT_PULLUP)
  SW(int pin,int mode) : SW_PIN(pin) {
    pinMode(SW_PIN, mode);
    // 長押しとみなす時間（ミリ秒）
    longPressDuration = 700;
    // ダブルクリックとみなす時間間隔（ミリ秒）
    doubleClickDelay = 300;
    Serial.println("SW setup was completed.");
  }
  //pin状態確認
  int check_a(){
    return digitalRead(SW_PIN);
  }
  // 状態変化有無
  bool check_change(){
    cur_value = digitalRead(SW_PIN);
    //前回値と今回値が異なるとき
    if(cur_value != last_value){
      sw_val = true;
    }else sw_val = false;
    //前回値を更新
    last_value = cur_value;       
    //
    if(sw_val)return true;
    else return false;
  }
  
  // ダブル:3 ,長押し:2,短押し:1,押していない:0
  int check_m() {
    cur_value = digitalRead(SW_PIN);

    if (cur_value != last_value) {
      last_value = cur_value;
      if (cur_value == LOW) {
        if ((millis() - lastDebounceTime) > longPressDuration) {
          buttonLongPressed = true;
          Serial.println("LONG");
          return 2;
        } else {
          int currentTime = millis();
          if (currentTime - lastClickTime < doubleClickDelay) {
            lastClickTime = 0;
            Serial.println("DOUBLE");
            return 3;
          } else {
            lastClickTime = currentTime;
            Serial.println("SINGLE");
            return 1;
          }
        }
      } else {
        lastDebounceTime = millis();
        buttonLongPressed = false;
      }
    }

    last_value = cur_value;

    return 0;
  }
  private:
    int cur_value = 0;
    int last_value = 0;

    unsigned long lastDebounceTime = 0;// 最後にボタンが押された時間
    unsigned long debounceDelay = 50;// デバウンス用の遅延時間（ミリ秒）
    bool buttonLongPressed = false;// ボタンが長押し状態かどうかを示すフラグ
    unsigned long longPressDuration = 700;// 長押しとみなす時間（ミリ秒）

    int lastClickTime = 0; // 最後のクリック時間
    int doubleClickDelay = 300;// ダブルクリックとみなす時間間隔（ミリ秒）
    bool sw_val = false;
};

#endif