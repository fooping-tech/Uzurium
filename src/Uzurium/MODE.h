#ifndef MODE_H
#define MODE_H

#include <M5Unified.h>
#include "DCMPWM.h"
#include "RINGLED.h"
#include "PHOTO.h"

class MODE{
  public:
    MODE(){//コンストラクタ
      TaskSpan=100;
      startTime = millis();
      cycleTime = millis();
      adv=0;
    } 
    ~MODE(){//デストラクタ
    }
    uint32_t CheckSpentTime();
    void mainloop();
    virtual void main();
    void GetAdValue(int value){
      adv = value;
    }
  protected:
    int TaskSpan;
    uint32_t startTime;
    uint32_t cycleTime;
    uint32_t deltaTime;
    uint32_t spentTime;
    int adv;

};


class ADinputMode : public MODE {
public:
    ADinputMode(PHOTO *p,DCMPWM *m,RINGLED *l) : MODE() {
     Serial.println("ADinputMode_begin");
     photo = p;
     motor = m;
     led = l;
    }

    // main関数をオーバーライド
    void main() override {
      // カスタムモードの処理を実装
      //RPMを計測する
      photo->CalcNowRPM();
      
      //シリアル通信のデータを書き出す
      //SERIAL_SetSerialData();
      //エッジがしばらく来ない場合にRPM初期化
      photo->CheckTimeout();
      //PID制御によりDUTYを計算する
      photo->ClacDuty(deltaTime);
      //BUZZERを鳴らしていない時
//      if(!BUZZER_CheckInit()){
        //算出したDUTYでモータを回す
        //motor.move(PHOTO_CheckDuty());
        //ポテンショ対応;DUTYを上書き
        int duty = map(adv,0,4095,0,200);
        motor->move(duty);
        //DUTYに応じてLEDのhueを可変
        led->fire2(4,duty+100);
//      }

    }

    private:
      DCMPWM *motor;
      PHOTO *photo;
      RINGLED *led;
      
        
  };
  
#endif