#ifndef MODE_H
#define MODE_H

#include <M5Unified.h>
#include "DCMPWM.h"
#include "RINGLED.h"
#include "PHOTO.h"

class MODE{
  public:
    MODE(PHOTO *p,DCMPWM *m,RINGLED *l){//コンストラクタ
      TaskSpan=100;
      startTime = millis();
      cycleTime = millis();
      adv=0;
      active=false;
      str="";
      photo = p;
      motor = m;
      led = l;
    }
    ~MODE(){//デストラクタ
    }
    uint32_t CheckSpentTime();
    void mainloop();
    virtual void main();
    bool active=false;
    char* str;
    void GetAdValue(int value){
      adv = value;
    }
    void InitFunction();
    
  protected:
    int TaskSpan;
    uint32_t startTime;
    uint32_t cycleTime;
    uint32_t deltaTime;
    uint32_t spentTime;
    int adv;
    bool _InitCondition=false;
    int _InitCounter=0;
    int _InitMode=0;
    DCMPWM *motor;
    PHOTO *photo;
    RINGLED *led;
};

/*
class BuzzerMode : public MODE {
public:
    BuzzerMode(int m) : MODE() {
     Serial.println("<----BuzzerMode_begin---->");
     active=true;
     seq=m;
    }
    // main関数をオーバーライド
    void main() override {
        motor->move(10);
        led->counter(photo->CheckCounter());
    }

    private:
        int seq=0;
        
};
*/
class ADinputMode : public MODE {
public:
    ADinputMode(PHOTO *p,DCMPWM *m,RINGLED *l) : MODE(p,m,l) {
     Serial.println("<----ADinputMode_begin---->");
     photo = p;
     motor = m;
     led = l;
     active=true;
     _InitMode=2;
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
      //算出したDUTYでモータを回す

      //ポテンショ対応;DUTYを上書き
      int duty = map(adv,0,4095,0,200);
      photo->SetDuty(duty);
      motor->move(photo->CheckDuty());
      //motor->move(duty);
      //DUTYに応じてLEDのhueを可変
      led->fire2(4,photo->CheckDuty()+100);

      //Serial.print(spentTime);
      //Serial.print(",");
      Serial.print(photo->CheckRpm());
      Serial.print(",");
      Serial.print(photo->CheckNowRPM());
      Serial.print(",");
      Serial.print(photo->CheckTargetRPM());
      Serial.print(",");
      Serial.println(photo->CheckDuty());

    }

    private:
      DCMPWM *motor;
      PHOTO *photo;
      RINGLED *led;
      
        
  };
class StopMode : public MODE {
public:
    StopMode(PHOTO *p,DCMPWM *m,RINGLED *l) : MODE(p,m,l) {
     Serial.println("<----StopMode_begin---->");
     photo = p;
     motor = m;
     led = l;
     active=false;
     _InitMode=0;//初期化時のブザーモード
    }
    // main関数をオーバーライド
    void main() override {
        motor->move(0);
        //DUTYに応じてLEDのhueを可変
        led->pacifica();
//      }

    }

    private:
  };

class FeedBackMode : public MODE {
public:
    FeedBackMode(PHOTO *p,DCMPWM *m,RINGLED *l) : MODE(p,m,l) {
     Serial.println("<----FeedBackMode_begin---->");
     photo = p;
     motor = m;
     led = l;
     active=true;
     startTime = millis();
     _InitMode=1;
    }
    // main関数をオーバーライド
    void main() override {
//RPMを計測する
      photo->CalcNowRPM();
      //シリアル通信のデータを書き出す
      //SERIAL_SetSerialData();
      //エッジがしばらく来ない場合にRPM初期化
      photo->CheckTimeout();
      //BUZZERを鳴らしていない時
//      if(!BUZZER_CheckInit()){
         //PID制御によりDUTYを計算する
        photo->ClacDuty(deltaTime);
        //算出したDUTYでモータを回す
        motor->move(photo->CheckDuty());
        //DUTYに応じてLEDのhueを可変
        led->fire2(4,photo->CheckDuty());
//      }
      //Kpをポテンショで可変
      float Kp = map(adv,0,4095,0,1000) * 0.000001;
//      Serial.print("Kp=");
//      Serial.println(Kp,8);
      photo->SetKp(Kp);


      Serial.print(photo->CheckRpm());
      Serial.print(",");
      Serial.print(photo->CheckNowRPM());
      Serial.print(",");
      Serial.print(photo->CheckTargetRPM());
      Serial.print(",");
      Serial.println(photo->CheckDuty());

      //脱調判定
      photo->CheckOutOfStep();
      //TargetRPM設定
      if(spentTime>1000)photo->SetTargetRPM(0);
      if(spentTime>3000) photo->SetTargetRPM(1000);
      if(spentTime>10000)photo->SetTargetRPM(1500);
      if(spentTime>15000)photo->SetTargetRPM(2000);
      if(spentTime>20000)photo->SetTargetRPM(2500);
      if(spentTime>25000)photo->SetTargetRPM(3000);
      if(spentTime>30000)photo->SetTargetRPM(3000);
      if(spentTime>35000)photo->SetTargetRPM(2500);
      if(spentTime>40000)photo->SetTargetRPM(1500);
      if(spentTime>45000)photo->SetTargetRPM(0);

      //脱調していたら
      if(photo->CheckOutFlag()){
      }

    }

    private:

};

class PhotoReflectorInspectionMode : public MODE {
public:
    PhotoReflectorInspectionMode(PHOTO *p,DCMPWM *m,RINGLED *l) : MODE(p,m,l) {
     Serial.println("<----PhotoReflectorInspectionMode_begin---->");
     photo = p;
     motor = m;
     led = l;
     active=true;
     _InitMode=3;
    }
    // main関数をオーバーライド
    void main() override {
        motor->move(0);
        led->counter(photo->CheckCounter());
    }

    private:

};

class LedInspectionMode : public MODE {
public:
    LedInspectionMode(PHOTO *p,DCMPWM *m,RINGLED *l) : MODE(p,m,l) {
     Serial.println("<----LedInspectionMode_begin---->");
     photo = p;
     motor = m;
     led = l;
     active=true;
    }
    // main関数をオーバーライド
    void main() override {
        motor->move(0);
        if(2000>=spentTime && spentTime>0)led->inspection(2);
        if(4000>=spentTime && spentTime>2000)led->inspection(0);
        if(6000>=spentTime && spentTime>4000)led->inspection(1);
        if(8000>=spentTime && spentTime>6000)led->inspection(2);
        if(10000>=spentTime && spentTime>8000)led->inspection(0);
        if(12000>=spentTime && spentTime>10000)led->inspection(1);
        if(14000>=spentTime && spentTime>12000)led->inspection(2);
        if(16000>=spentTime && spentTime>14000)led->inspection(0);
        if(18000>=spentTime && spentTime>16000)led->inspection(1);
        if(spentTime>18000)led->inspection(2);
    }

    private:

};
#endif