/*LCD螢幕顯示*/

void fourthTask() {
  if (millis() - Now_time <= 1500) {
    lcd_i2c.setCursor(0,0);
    lcd_i2c.print(String() + "TEMP:" + Var_temp + "  HUMI:" + Var_humi);
    lcd_i2c.setCursor(0,1);
    lcd_i2c.print(String() + "S:" + Var_shock + "  RAIN:" + Var_rain);

  }
  if ((millis() - Now_time > 1500) && (millis() - Now_time <= 3000)) {
    lcd_i2c.clear();
    lcd_i2c.setCursor(0,0);
    lcd_i2c.print(String() + "PIR:" + Var_pir + " LIGHT:" + Var_light_sensor);

  }
  if (millis() - Now_time > 3000) {
    Now_time = millis();
  }
}

