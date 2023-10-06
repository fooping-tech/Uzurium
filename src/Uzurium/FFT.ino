#include "arduinoFFT.h"
arduinoFFT FFT;

const uint16_t FFT_SAMPLES = 128; // サンプル数(2^x)
const int FFT_TaskSpan = 100; // タスク実行間隔(ms)
const int FFT_TaskSpan_us = 10; // タスク実行間隔(us)
int FFT_ADvalue=0;

double vReal[FFT_SAMPLES];  //FFT入力データ
double vImag[FFT_SAMPLES];  //FFT入力データ
double vBuff[FFT_SAMPLES];  //バッファ
double vTime[FFT_SAMPLES];  //サンプリング間隔[us]

//uint32_t FFT_startTime = 0;//スタートタイム[ms]
uint32_t FFT_cycleTime = 0;//サイクルタイム[ms]
uint32_t FFT_cycleTime_us = 0;//サイクルタイム[us]

bool FFT_Initialized = false;//初期化フラグ
double FFT_SamplingFrequency = 1000000 / FFT_TaskSpan_us;//サンプリング周期

volatile double FFT_Peak=0;  //FFTピーク周波数
volatile int FFT_Magnitude=0;//音量

//初期化処理
void FFT_Init(){
    //startTimeに現在時刻を設定
    //FFT_startTime = millis();
    //サイクルタイムに現在時刻を設定
    FFT_cycleTime = millis();
    //pinモードセット
    pinMode(ANALOG_PIN,INPUT);
    //初期化フラグを立てる
    FFT_Initialized = true;
}
//終了時
void FFT_Finish(){
    //初期化フラグを下す
    FFT_Initialized = false;
}
double FFT_CheckPeak(){
  return FFT_Peak;
}
int FFT_CheckMagnitude(){
  //DUMP(FFT_Magnitude);
  return FFT_Magnitude;
}
void FFT_Task(void *pvParameters){
  while(1){
    FFT_main();
    delay(1);
  }
}
//FFTメイン処理
void FFT_main(){

  if(!FFT_Initialized){
    //未初期化のとき
    FFT_Init();
  }else{
    //初期化済みの時
    //前回実行時からの経過時間を計算
    uint32_t deltaTime = millis() - FFT_cycleTime;
    uint32_t deltaTime_us = micros() - FFT_cycleTime_us;
    //経過時間を計算
    //uint32_t spentTime = millis() - FFT_startTime;
    
    //所定時間以上経過していたら実行
    if(deltaTime_us >=FFT_TaskSpan_us){
      //計測
      FFT_Measure(deltaTime_us);
      //cycleTimeリセット
      FFT_cycleTime_us = micros();
    }
    
    //所定時間以上経過していたら実行
    if(deltaTime >= FFT_TaskSpan){
      FFT_CalcFFT();
      //cycleTimeリセット
      FFT_cycleTime = millis();
    }
  }
}

//計測
void FFT_Measure(uint32_t deltaTime_us){
  //Analogpinからデータ読み出し
  int value = analogRead(ANALOG_PIN);
  FFT_ADvalue = value;
  //FFT用バッファにデータをプッシュ
  FFT_PushData(value);
  //サンプリング平均間隔[us]を算出
  int ave = FFT_AverageTime(deltaTime_us);
  //Serial.println(ave);
  //サンプリング周波数を計算
  FFT_SamplingFrequency=1000000 / ave;
}

void FFT_CalcFFT(){

  //テスト用波形生成
  /*
  const double signalFrequency = 1000;
  const uint8_t amplitude = 64;
  const double   pi                = 3.14159265358979324;
  for (uint16_t i = 0; i < FFT_SAMPLES; i++)
  {
    vReal[i] = amplitude * (sin(i * 2.0 * pi * signalFrequency / FFT_SamplingFrequency));
    vImag[i] = 0.0;
  }
  */

  //バッファからデータ読み込み
  for(int i=0; i<FFT_SAMPLES;i++){
    vReal[i] = vBuff[i];
    vImag[i] = 0.0;
  }

  //FFT
  FFT = arduinoFFT(vReal, vImag, FFT_SAMPLES, FFT_SamplingFrequency);
  FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);	// Weigh data
  FFT.Compute(FFT_FORWARD); // Compute FFT
  FFT.ComplexToMagnitude(); // Compute magnitudes
  
  //ピーク検出
  FFT_Peak = FFT.MajorPeak();
  //音量検出
  FFT_Magnitude = FFT_CalcMagnitude();
  
  //音量が小さい場合はFFT結果を表示しない
  //if( FFT_Magnitude< 50)FFT_Peak = 0;

  //シリアル表示
  
  //Serial.print(FFT_Magnitude);
  //Serial.print(" ");
  //Serial.println(FFT_Peak, 6);
  
}



//音量計測
int FFT_CalcMagnitude(){
  int sum =0;
  for(int i=0; i<FFT_SAMPLES;i++){
    sum += vBuff[i];
  }
  int ave = sum/FFT_SAMPLES;
  sum=0;
  for(int i=0; i<FFT_SAMPLES;i++){
    sum += abs(vBuff[i]-ave);
  }
  return sum/FFT_SAMPLES;
}

//値を1つずらして0に新しい値を入れる
void FFT_PushData(int new_value){
  for (int i = 0; i < FFT_SAMPLES -1; i++){
    vBuff[i] = vBuff[i + 1];
  }
  vBuff[FFT_SAMPLES-1] = new_value;
}

//平均値を返す
int FFT_AverageTime(uint32_t new_value){
  for (int i = 0; i < FFT_SAMPLES -1; i++){
    vTime[i] = vTime[i + 1];
  }
  vTime[FFT_SAMPLES-1] = new_value;
  uint32_t sum = 0;
  for (int i = 0; i < FFT_SAMPLES; i++){
    sum += vTime[i];
  }
  return sum / FFT_SAMPLES;
}

/*
//FFTデータ表示用関数
#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03
void FFT_PrintVector(double *vData, uint16_t bufferSize, uint8_t scaleType)
{
  for (uint16_t i = 0; i < bufferSize; i++)
  {
    //横座標
    double abscissa;
    switch (scaleType)
    {
      case SCL_INDEX:
        abscissa = (i * 1.0);
        break;
      case SCL_TIME:
        abscissa = ((i * 1.0) / FFT_SamplingFrequency);
        break;
      case SCL_FREQUENCY:
        abscissa = ((i * 1.0 * FFT_SamplingFrequency) / FFT_SAMPLES);
        break;
    }
    Serial.print(abscissa, 6);
    if(scaleType==SCL_FREQUENCY) Serial.print("Hz");//横軸周波数の場合はHz付与
    Serial.print(" ");
    Serial.println(vData[i], 4);
  }
  Serial.println();
}
*/