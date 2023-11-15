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
      active=true;
      name="";
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
    char* name;
    void SetAdValue(int value){
      adv = value;
    }
    void SetParams(int d,int h,int b){
      _duty =d;
      _hue = h;
      _brightness = b;
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
    int _value=0;
    DCMPWM *motor;
    PHOTO *photo;
    RINGLED *led;
    int _duty=0;
    int _hue=0;
    int _brightness=0;
};

class ADinputMode : public MODE {
public:
    ADinputMode(PHOTO *p,DCMPWM *m,RINGLED *l) : MODE(p,m,l) {
     Serial.println("<----ADinputMode_begin---->");
     photo = p;
     motor = m;
     led = l;
     led->setbrightness(25);
     name="ADinputMode";
     _InitMode=2;
     _value=100;//ボタン選択時の色
    }
  
    // main関数をオーバーライド
    void main() override {
      // カスタムモードの処理を実装
      //RPMを計測する
      photo->CalcNowRPM();
      //エッジがしばらく来ない場合にRPM初期化
      photo->CheckTimeout();
      //PID制御によりDUTYを計算する
      photo->ClacDuty(deltaTime);
      //ポテンショ対応;DUTYを上書き
      int duty = map(adv,0,4095,0,200);
      photo->SetDuty(duty);
      motor->move(photo->CheckDuty());
      //motor->move(duty);
      //DUTYに応じてLEDのhueを可変
      led->fire2(4,photo->CheckDuty()+100);

      Serial.print("RPM:");
      Serial.print(photo->CheckRpm());
      Serial.print(",");
      Serial.print("nowRPM:");
      Serial.print(photo->CheckNowRPM());
      Serial.print(",");
      Serial.print("TargetRPM:");
      Serial.print(photo->CheckTargetRPM());
      Serial.print(",");
      Serial.print("DUTY:");
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
     led->setbrightness(25);
     name="StopMode";
     _InitMode=0;//初期化時のブザーモード
    _value=150;//ボタン選択時の色

    }
    // main関数をオーバーライド
    void main() override {
        motor->move(0);
        //DUTYに応じてLEDのhueを可変
        led->pacifica();
    }

    private:
  };

class TestMode : public MODE {
public:
    TestMode(PHOTO *p,DCMPWM *m,RINGLED *l) : MODE(p,m,l) {
     Serial.println("<----TestMode_begin---->");
     photo = p;
     motor = m;
     led = l;
     led->setbrightness(25);
     name="TestMode";
     _InitMode=6;//初期化時のブザーモード
    _value=150;//ボタン選択時の色
    }
    // main関数をオーバーライド
    void main() override {
        motor->move(0);
        //DUTYに応じてLEDのhueを可変
        led->flash(50);
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
     led->setbrightness(25);
     startTime = millis();
     _InitMode=1;
     TaskSpan=10;
     name="FeedBackMode";
    _value=200;//ボタン選択時の色
    }
    // main関数をオーバーライド
    void main() override {
//RPMを計測する
      photo->CalcNowRPM();
      //エッジがしばらく来ない場合にRPM初期化
      photo->CheckTimeout();
         //PID制御によりDUTYを計算する
        photo->ClacDuty(deltaTime);
        //算出したDUTYでモータを回す
        motor->move(photo->CheckDuty());
        //DUTYに応じてLEDのhueを可変
        led->fire2(4,photo->CheckDuty());
//      }
      //Kpをポテンショで可変
      float Kp = map(adv,0,4095,0,1000) * 0.000001;

      photo->SetKp(Kp);

      Serial.print("RPM:");
      Serial.print(photo->CheckRpm());
      Serial.print(",");
      Serial.print("nowRPM:");
      Serial.print(photo->CheckNowRPM());
      Serial.print(",");
      Serial.print("TargetRPM:");
      Serial.print(photo->CheckTargetRPM());
      Serial.print(",");
      Serial.print("DUTY:");
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
      if(spentTime>50000)active=false;

      //脱調していたら
      if(photo->CheckOutFlag()){
      }

    }

    private:

};

//=========Inspection Functions============

class ADInspectionMode : public MODE {
public:
    ADInspectionMode(PHOTO *p,DCMPWM *m,RINGLED *l) : MODE(p,m,l) {
     Serial.println("<----ADInspectionMode_begin---->");
     photo = p;
     motor = m;
     led = l;
     led->setbrightness(10);
     _InitMode=5;
     name="ADInspectionMode";
    _value=210;//ボタン選択時の色

    }
    // main関数をオーバーライド
    void main() override {
        motor->move(0);
        int value = map(adv,0,4095,1,18);
        led->level(value);
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
     led->setbrightness(10);
     _InitMode=3;
     name="PhotoReflectorInspectionMode";
    _value=220;//ボタン選択時の色

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
     led->setbrightness(10);
     _InitMode=4;
     name="LedInspectionMode";
    _value=230;//ボタン選択時の色

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
        if(spentTime>20000)active=false;
    }

    private:

};

class RemoteControlMode : public MODE {
public:
    RemoteControlMode(PHOTO *p,DCMPWM *m,RINGLED *l) : MODE(p,m,l) {
     Serial.println("<----RemoteControlMode_begin---->");
     photo = p;
     motor = m;
     led = l;
     led->setbrightness(25);
     _InitMode=0;//初期化時のブザーモード
     name="RemoteControlMode";
    _value=250;//ボタン選択時の色

    }
    // main関数をオーバーライド
    void main() override {
        motor->move(_duty);
        //DUTYに応じてLEDのhueを可変
        led->fire2(4,_hue);
        led->setbrightness(_brightness);
    }
  private:

  };
#endif