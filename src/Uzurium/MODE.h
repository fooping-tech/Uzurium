#ifndef MODE_H
#define MODE_H

//#include <M5Unified.h>
#include <M5Atom.h>

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
    void InitActive();
    void InitFunction();
    void DeinitActive();
    void DeinitFunction();
    
    
  protected:
    int TaskSpan;       //タスク実行間隔
    uint32_t startTime; //開始時間
    uint32_t cycleTime; //サイクルタイム
    uint32_t deltaTime; //
    uint32_t spentTime; //経過時間
    int adv;            //AD value
    bool _InitCondition=false;  //初期化状態
    int _InitCounter=0;         //カウンタ
    int _InitMode=0;            //初期化モード
    int _value=0;                 //ボタン選択時の色
    bool _DeinitCondition=false;  //終了時状態
    int _DeinitCounter=0;         //終了時カウンタ
    int _DeinitMode=0;            //終了時モード

    DCMPWM *motor;        //モータインスタンスのポインタ
    PHOTO *photo;         //フォトインスタンスのポインタ
    RINGLED *led;         //LEDインスタンスのポインタ
    int _duty=0;
    int _hue=0;
    int _brightness=0;
    //int _MaxDuty=100;
};

class ADinputMode : public MODE {
public:
    ADinputMode(PHOTO *p,DCMPWM *m,RINGLED *l) : MODE(p,m,l) {

     photo = p;
     motor = m;
     led = l;
     led->setbrightness(25);
     name="ADinputMode";
     _InitMode=2;
     _value=100;//ボタン選択時の色
     Serial.print("<----");
     Serial.print(name);
     Serial.println("_begin---->");
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
      int duty = map(adv,0,4095,0,photo->CheckMaxDuty());
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

     photo = p;
     motor = m;
     led = l;
     led->setbrightness(25);
     name="StopMode";
     _InitMode=0;//初期化時のブザーモード
    _value=150;//ボタン選択時の色
     Serial.print("<----");
     Serial.print(name);
     Serial.println("_begin---->");
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

     photo = p;
     motor = m;
     led = l;
     led->setbrightness(25);
     name="TestMode";
     _InitMode=6;//初期化時のブザーモード
    _value=150;//ボタン選択時の色
     Serial.print("<----");
     Serial.print(name);
     Serial.println("_begin---->");
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

     photo = p;
     motor = m;
     led = l;
     led->setbrightness(25);
     startTime = millis();
     _InitMode=1;
     TaskSpan=10;
     name="FeedBackMode";
    _value=200;//ボタン選択時の色
    _DeinitMode=8;
     Serial.print("<----");
     Serial.print(name);
     Serial.println("_begin---->");
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
        led->fire2(4,photo->CheckDuty()*5);
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
      if(spentTime>0)photo->SetTargetRPM(1000);
      if(spentTime>3000) photo->SetTargetRPM(500);
      if(spentTime>4000) photo->SetTargetRPM(3000);
      if(spentTime>10000)photo->SetTargetRPM(1000);
      if(spentTime>13000) photo->SetTargetRPM(500);
      if(spentTime>14000)photo->SetTargetRPM(3000);
      if(spentTime>20000){
        photo->SetTargetRPM(1000);
      }
      if(spentTime>23000) photo->SetTargetRPM(500);
      if(spentTime>24000)photo->SetTargetRPM(3000);
      if(spentTime>30000)photo->SetTargetRPM(1000);
      if(spentTime>33000) photo->SetTargetRPM(500);
      if(spentTime>34000)photo->SetTargetRPM(3000);
      if(spentTime>40000){
        photo->SetTargetRPM(1000);
      }
      if(spentTime>43000) photo->SetTargetRPM(500);
      if(spentTime>44000)photo->SetTargetRPM(3000);
      if(spentTime>50000)photo->SetTargetRPM(1000);
      if(spentTime>53000) photo->SetTargetRPM(500);
      if(spentTime>54000)photo->SetTargetRPM(3000);
      if(spentTime>60000){
        DeinitActive();
      }

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

     photo = p;
     motor = m;
     led = l;
     led->setbrightness(10);
     _InitMode=5;
     name="ADInspectionMode";
    _value=210;//ボタン選択時の色
     Serial.print("<----");
     Serial.print(name);
     Serial.println("_begin---->");
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

     photo = p;
     motor = m;
     led = l;
     led->setbrightness(10);
     _InitMode=3;
     name="PhotoReflectorInspectionMode";
    _value=100;//ボタン選択時の色
     Serial.print("<----");
     Serial.print(name);
     Serial.println("_begin---->");
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

     photo = p;
     motor = m;
     led = l;
     led->setbrightness(10);
     _InitMode=4;
     name="LedInspectionMode";
    _value=230;//ボタン選択時の色
     Serial.print("<----");
     Serial.print(name);
     Serial.println("_begin---->");
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

     photo = p;
     motor = m;
     led = l;
     led->setbrightness(25);
     _InitMode=0;//初期化時のブザーモード
     name="RemoteControlMode";
    _value=250;//ボタン選択時の色
     Serial.print("<----");
     Serial.print(name);
     Serial.println("_begin---->");
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

class TimerMode : public MODE {
  public:
    TimerMode(PHOTO *p,DCMPWM *m,RINGLED *l) : MODE(p,m,l) {
     photo = p;
     motor = m;
     led = l;
     led->setbrightness(25);
     _InitMode=0;//初期化時のブザーモード
     name="TimerMode";
    _value=250;//ボタン選択時の色
    _time=6 * 1000;//タイマー設定
    _counter=1;
    _hue = 100;
    _DeinitMode=7;

     Serial.print("<----");
     Serial.print(name);
     Serial.println("_begin---->");

    }
    // main関数をオーバーライド
    void main() override {
      int max_duty = map(adv,0,4095,0,photo->CheckMaxDuty());
      int duty = 0;
/*
      led->fire2(4,_hue*_counter);
      if(spentTime>_time*0*_counter/3)duty= max_duty *0.3;
      if(spentTime>_time*0.2*_counter/3)duty= max_duty *0.2;
      if(spentTime>_time*0.3*_counter/3)duty= max_duty *0.4;
      if(spentTime>_time*0.4*_counter/3)duty= max_duty *0.3;
      if(spentTime>_time*0.5*_counter/3)duty= max_duty *0.5;
      if(spentTime>_time*0.6*_counter/3)duty= max_duty *0.4;
      if(spentTime>_time*0.7*_counter/3)duty= max_duty *0.6;
      if(spentTime>_time*0.8*_counter/3)duty= max_duty *0.5;
      if(spentTime>_time*0.9*_counter/3)duty= max_duty *1.0;
      if(spentTime>_time*1.0*_counter/3){
        _counter++;
      }
*/
      float low=0.3;
      float middle=0.5;
      float high=1;
      int b_low = 25;
      int b_high = 30;

      
      if(spentTime>0)duty= max_duty * middle;
      if(spentTime>3000) duty= max_duty * low;
      if(spentTime>4000){
        led->setbrightness(b_high);
        duty= max_duty * high;
      }
      if(spentTime>10000){
        led->setbrightness(b_low);
        duty= max_duty * middle;
      }
      if(spentTime>13000)duty= max_duty * low;
      if(spentTime>14000){
        led->setbrightness(b_high);
        duty= max_duty * high;
      }
      if(spentTime>20000){
        led->setbrightness(b_low);
        duty= max_duty * middle;
      }
      if(spentTime>23000) duty= max_duty * low;
      if(spentTime>24000){
        led->setbrightness(b_high);
        duty= max_duty * high;
      }
      if(spentTime>30000){
        led->setbrightness(b_low);
        duty= max_duty * middle;
      }
      if(spentTime>33000) duty= max_duty * low;
      if(spentTime>34000){
        led->setbrightness(b_high);
        duty= max_duty * high;
      }
      if(spentTime>40000){
        led->setbrightness(b_low);
        duty= max_duty * middle;
      }
      if(spentTime>43000) duty= max_duty * low;
      if(spentTime>44000){
        led->setbrightness(b_high);
        duty= max_duty * high;
      }
      if(spentTime>50000){
        led->setbrightness(b_low);
        duty= max_duty * middle;
      }
      if(spentTime>53000)duty= max_duty * low;
      if(spentTime>54000){
        led->setbrightness(b_high);
        duty= max_duty * high;
      }
      if(spentTime>60000){
        _hue=200;
        led->setbrightness(b_low);
        duty= max_duty * middle;
      }
      if(spentTime>63000)  duty= max_duty * low;
      if(spentTime>64000){
        led->setbrightness(b_high);
        duty= max_duty * high;
      }
      if(spentTime>70000){
        led->setbrightness(b_low);
        duty= max_duty * middle;
      }
      if(spentTime>73000) duty= max_duty * low;
      if(spentTime>74000){
        led->setbrightness(b_high);
        duty= max_duty * high;
      }
      if(spentTime>80000){
        led->setbrightness(b_low);
        duty= max_duty * middle;
      }
      if(spentTime>83000) duty= max_duty * low;
      if(spentTime>84000){
        led->setbrightness(b_high);
        duty= max_duty * high;
      }
      if(spentTime>90000){
        led->setbrightness(b_low);
        duty= max_duty * middle;
      }
      if(spentTime>93000) duty= max_duty * low;
      if(spentTime>94000){
        led->setbrightness(b_high);
        duty= max_duty * high;
      }
      if(spentTime>100000){
        led->setbrightness(b_low);
        duty= max_duty * middle;
      }
      if(spentTime>103000)duty= max_duty * low;
      if(spentTime>104000){
        led->setbrightness(b_high);
        duty= max_duty * high;
      }
      if(spentTime>110000){
        led->setbrightness(b_low);
        duty= max_duty * middle;
      }
      if(spentTime>113000)duty= max_duty * low;
      if(spentTime>114000){
        led->setbrightness(b_high);
        duty= max_duty * high;
      }
      if(spentTime>120000){
        _hue=300;
        duty= max_duty * middle;
      }
      if(spentTime>123000)duty= max_duty * low;
      if(spentTime>124000){
        led->setbrightness(b_high);
        duty= max_duty * high;
      }
      if(spentTime>130000){
        led->setbrightness(b_low);
        duty= max_duty * middle;
      }
      if(spentTime>133000)duty= max_duty * low;
      if(spentTime>134000){
        led->setbrightness(b_high);
        duty= max_duty * high;
      }
      if(spentTime>140000){
        led->setbrightness(b_low);
        duty= max_duty * middle;
      }
      if(spentTime>143000)duty= max_duty * low;
      if(spentTime>144000){
        led->setbrightness(b_high);
        duty= max_duty * high;
      }
      if(spentTime>150000){
        led->setbrightness(b_low);
        duty= max_duty * middle;
      }
      if(spentTime>153000)duty= max_duty * low;
      if(spentTime>154000){
        led->setbrightness(b_high);
        duty= max_duty * high;
      }
      if(spentTime>160000){
        led->setbrightness(b_low);
        duty= max_duty * middle;
      }
      if(spentTime>163000)duty= max_duty * low;
      if(spentTime>164000){
        led->setbrightness(b_high);
        duty= max_duty * high;
      }
      if(spentTime>170000){
        led->setbrightness(b_low);
        duty= max_duty * middle;
      }
      if(spentTime>173000)duty= max_duty * low;
      if(spentTime>174000){
        led->setbrightness(b_high);
        duty= max_duty * high;
      }
      if(spentTime>180000){
        duty = 0;
        DeinitActive();
      }
      photo->SetDuty(duty);
      motor->move(photo->CheckDuty());
      led->fire2(4,_hue+duty);


    }
  private:
   int _time; //timer
   int _counter;

};

#endif