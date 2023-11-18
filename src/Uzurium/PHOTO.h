#ifndef PHOTO_H
#define PHOTO_H

#include <M5Unified.h>

#define STOCK_RPMS 5

class PHOTO{
  static volatile int counter;
  static volatile unsigned long lastPulseTime;
  static volatile unsigned long pulseInterval;
  static volatile uint16_t rpm; // RPMの変数 uint16_t:65535rpmまで計測可能
  public:
    PHOTO();
    void setup(int);
    void reset();
    static void StopInterrupt();
    static void SetInterrupt();
    void CalcRPM();
    void SetTargetRPM(float);
    float CheckTargetRPM();
    float CheckRpm();
    float CheckNowRPM();
    float CheckOutRPM(float);
    float CheckInRPM(float);
    float CheckDiffRPM();
    void CalcNowRPM();
    void DeleteNowRPMStock();
    int CheckDuty();
    int CheckDuty_p();
    bool CheckTimeout();
    void CheckOutOfStep();
    bool CheckOutFlag();
    void SetDuty(int);
    void SetOffsetDuty(int);
    void ClacDuty(uint32_t);
    void IncreaseDuty(int);
    void DecreaseDuty(int);
    int CheckCounter();
    void SetKp(float);
    static void Measure(); //digitalPinToInterruptの引数で読み込むために静的メンバ関数として定義

  private:
    bool _isSetuped;
    const int _OffsetDutyInitValue;//初期値DUTY  = 0
    const int _LimitRPMD; //脱調判定回転数変化= 800
    const int _StartRPMD;//回転開始回転数変化 = 10
    const int _LowRPM; //低回転判定閾値 = 100
    const int _MAX_DUTY; //0~256
    const int _MIN_DUTY; //0~256 =0
    const int _M_DUTY; //回転開始DUTYマージン = 0

    static int photo_pin;

    float outRPM;//脱調RPM=6000
    float inRPM;//回転開始RPM=0
    int outDuty;//脱調DUTY=0
    int inDuty;//回転開始DUTY=0

    float P,I,D,preP;
    float duty_p;//=0
    int duty;//=0

    float Kp = 0.0007;
    const float Ki = 0;
    const float Kd = 0;


    float nowRPM;// =0
    float beforeRPM;//=0
    float diffRPM;//=0
    float TargetRPM;// = 0
    int offset;//オフセットduty=0

    bool outFlag;//脱調フラグ =false
    bool inFlag;//回転開始フラグ =false
    int stock_rpm[STOCK_RPMS];
    int stock_rpm_num;//=0

};

#endif