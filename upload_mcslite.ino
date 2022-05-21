/*上傳感測器數值到MCSLITE*/

void fifthTask() {
  pir.set(Var_pir);
  shock.set(Var_shock);
  rain.set(Var_rain);
  indoor_light.set(Var_light_sensor);
  indoor_temp.set(Var_temp);
  indoor_hump.set(Var_humi);
  delay(500);
}

