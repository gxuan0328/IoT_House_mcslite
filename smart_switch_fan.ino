/*智慧風扇開關偵測*/

void secondTask() {
   while (!mcs.connected()) {
    mcs.connect();
    if (mcs.connected()) {
      Serial.println("MCS Reconnected.");
    }
  }
  mcs.process(100);
  
  if (switch_fan.value() == 1) {
    if (Var_temp >= Set_temp_value) {
      digitalWrite(Fan_pin, 0);
    } else {
      digitalWrite(Fan_pin, 1);
    }
  } else {
    if (on_off_fan.value() == 1) {
      digitalWrite(Fan_pin, 0);
    } else {
      digitalWrite(Fan_pin, 1);
    }
  }
  Serial.print(String() + "風扇手動開關: " + on_off_fan.value()+"\t");
  Serial.println(String() + "風扇智慧開關:  " + switch_fan.value());
}
