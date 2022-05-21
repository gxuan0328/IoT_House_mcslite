/*智慧電燈開關偵測*/

void thirdTask() {
  int i;
   while (!mcs.connected()) {
    mcs.connect();
    if (mcs.connected()) {
      Serial.println("MCS Reconnected.");
    }
  }
  mcs.process(100);
  if (switch_ws2812.value() == 1) {
    if (Var_light_sensor < 40) {
      for (i = 0; i <= 11; i++) {
        pixels.setPixelColor(i, pixels.Color(255, 255, 255));
      }
      pixels.show();
      digitalWrite(WS2812_led_pin, HIGH);
    } else {
      
      for (i = 0; i <= 11; i++) {
        pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      }
      pixels.show();
      digitalWrite(WS2812_led_pin, LOW);
    }
  } else {
    if (on_off_ws2812.value() == 1) {
      for (i = 0; i <= 11; i++) {
        pixels.setPixelColor(i, pixels.Color(255, 255, 255));
      }
      pixels.show();
      digitalWrite(WS2812_led_pin, HIGH);
    } else {
      for (i = 0; i <= 11; i++) {
        pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      }
      pixels.show();
      digitalWrite(WS2812_led_pin, LOW);
    }
  }
  Serial.print(String() + "燈環手動開關: " + on_off_ws2812.value()+"\t");
  Serial.println(String() + "燈環智慧開關:  " + switch_ws2812.value());
}
