int cur_value = 0;
int last_value = 0;

  void SW_setup(){
   pinMode(SW_PIN, INPUT);
   Serial.println("SW setup was completed.");
  }

  bool SW_check(){
  
    cur_value = digitalRead(SW_PIN);
  
    if(cur_value != last_value){  //前回値と今回値が異なり
      last_value = cur_value;       //前回値を更新
      if(cur_value==0){               //今回0のとき(0がSW=0N)
        return true;
      }else return false;
    }else{
          last_value = cur_value;       //前回値を更新
          return false;
    }


  }