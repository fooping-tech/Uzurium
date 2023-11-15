int cur_value_sw = 0;
int last_value_sw = 0;
bool sw_val = false;

bool BUTTON_check_sw(){
  
  cur_value_sw = digitalRead(TEST_SW_PIN);
  if(cur_value_sw != last_value_sw){  //前回値と今回値が異なり
    sw_val = true;
  }else sw_val = false;

  last_value_sw = cur_value_sw;       //前回値を更新
  if(sw_val)return true;
  else return false;
}
/*
#define TopButton 1
#define RedButton 2
#define BlueButton 3
int cur_value_red = 0;
int cur_value_blue = 0;
int last_value_red = 0;
int last_value_blue = 0;

  void BUTTON_setup(){
   pinMode(TopButton, INPUT);
   pinMode(RedButton, INPUT);// Red Button Pin setting
   pinMode(BlueButton, INPUT);// Blue Button Pin setting
  }

  bool BUTTON_check_red(){
  
    cur_value_red = digitalRead(RedButton);
  
    if(cur_value_red != last_value_red){  //前回値と今回値が異なり
      if(cur_value_red==0){               //今回0のとき
        return true;
      }else return false;
    }
    last_value_red = cur_value_red;       //前回値を更新

  }

  bool BUTTON_check_blue(){
  
    cur_value_blue = digitalRead(BlueButton);

    if(cur_value_blue != last_value_blue){
      if(cur_value_blue==0){               //今回0のとき
        return true;
      }else return false;
    }
    last_value_blue = cur_value_blue;
  }

  void BUTTON_check_M5_button(){
    if (M5.BtnA.wasPressed()) {    //If the button is pressed
      Serial.println("Button is pressed.");
    }
  }
  */