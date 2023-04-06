
void INA219_setup(){
//INA219
  if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }

}

//Average Current
#define STOCK_CURRENT 15
int stock_current[STOCK_CURRENT];
int stock_current_num=0;

float INA219_AverageCurrent(float current_mA){
  stock_current_num++;
  if(stock_current_num >= STOCK_CURRENT){
    stock_current_num=0;
  }
  stock_current[stock_current_num]=current_mA;  
  float result=0;
  for(int i = 0;i <STOCK_CURRENT;i++){
    result +=stock_current[i];
  }
  result = result/STOCK_CURRENT;
  return result;
}