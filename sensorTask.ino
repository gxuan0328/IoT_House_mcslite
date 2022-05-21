/*RFID及震動感測器偵測*/

void firstTask() {  
  if (Var_rfid_string != "") {
    Serial.print("偵測到RFID:");
    Serial.println(Var_rfid_string);
    lcd_i2c.clear();
    lcd_i2c.setCursor(0, 0);
    lcd_i2c.print("You RFID: ");
    lcd_i2c.setCursor(0, 1);
    lcd_i2c.print(Var_rfid_string);
    myservo.write(90);
    delay(1000);
    myservo.write(0);
  }
  if (Var_shock > Set_shock_value) {
    tone(Buzzer_pin, 262);
  } else {
    noTone(Buzzer_pin);
  }
  if (Var_rain == 1 || (Var_shock > Set_shock_value || Var_pir == 1)) {
    digitalWrite(Red_led_pin, HIGH);
  } else {
    digitalWrite(Red_led_pin, LOW);
  }
  read_RFID.set(Var_rfid_string);
  //delay(1000);
}
