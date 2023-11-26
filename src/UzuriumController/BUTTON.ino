
//#define TopButton 1
#define RedButton 32
#define BlueButton 33
int cur_value_red = 0;
int cur_value_blue = 0;
int last_value_red = 0;
int last_value_blue = 0;

  void BUTTON_setup(){
   //pinMode(TopButton, INPUT);
   pinMode(RedButton, INPUT);// Red Button Pin setting
   pinMode(BlueButton, INPUT);// Blue Button Pin setting
  }

  bool BUTTON_check_red(){
  
    bool checkBtn =false;
    cur_value_red = digitalRead(RedButton);
    if(cur_value_red != last_value_red){  //前回値と今回値が異なり
      if(cur_value_red==0){               //今回0のとき
        checkBtn = true;
      }else checkBtn = false;
    }
    last_value_red = cur_value_red;       //前回値を更新
    return checkBtn;
  }

  bool BUTTON_check_blue(){
  
    bool checkBtn =false;
    cur_value_blue = digitalRead(BlueButton);

    if(cur_value_blue != last_value_blue){
      if(cur_value_blue==0){               //今回0のとき
        checkBtn = true;
      }else checkBtn = false;
    }
    last_value_blue = cur_value_blue;
    return checkBtn;
  }

