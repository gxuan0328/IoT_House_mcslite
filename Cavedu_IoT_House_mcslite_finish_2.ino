/****************************************/
/* Copyright (c) 2021 EricHsu in CAVEDU */
/****************************************/

/*匯入函示庫*/
#include <Servo.h>
Servo myservo;
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd_i2c(0x27);
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(12, 7, NEO_GRB + NEO_KHZ800);
#include <LWiFi.h>
#include "MCS.h"
#include <SPI.h>
#include <MFRC522.h>
#include <DHT.h>

/*感測器腳位*/
#define PIR_pin           2
#define Fan_pin           3
#define Rain_sensor_pin   4
#define Red_led_pin       5
#define SG90_servo_pin    6
#define WS2812_led_pin    7
#define RFID_pin         10
#define Light_sensor_pin 14
#define Dht11_pin        15
#define Buzzer_pin       16
#define Shocked_pin      17

/*溫度數值控制設定*/
#define Set_temp_value   28

/*震動感測器數值設定*/
#define Set_shock_value  25


/*設定感測器變數型態*/
int Var_pir;
int Var_rain;
String Var_rfid_string;
int Var_light_sensor;
int Var_temp;
int Var_humi;
int Var_shock;

int Now_time;


/*輸入雲端資訊*/
char _lwifi_ssid[]              = "MS_AGV_Car";                                                     //輸入WIFI帳號，不可輸入錯誤
char _lwifi_pass[]              = "12345678";                                                       //輸入WIFI密碼，不可輸入錯誤
#define _your_MCSLite_DeviceId   "SJ3CpUvWK"                                                        //輸入MCSLite的DeviceId
#define _your_MCSLite_DeviceKey  "36654dfd56b31155b55570ba55bf54fcea0d7c082ee7a085d51fa3e16968277d" //輸入MCSLite的Devicekey
#define _your_MCSLite_IP         "192.168.50.28" //輸入MCSLite的IP                         
MCSLiteDevice mcs(_your_MCSLite_DeviceId , _your_MCSLite_DeviceKey, _your_MCSLite_IP, 3000);

/*MCSLITE通道設置*/
MCSControllerOnOff on_off_ws2812("on_off_ws2812");
MCSControllerOnOff on_off_fan("on_off_fan");
MCSControllerOnOff switch_ws2812("switch_ws2812");
MCSControllerOnOff switch_fan("switch_fan");
MCSDisplayString read_RFID("read_RFID");
MCSDisplayInteger pir("pir");
MCSDisplayInteger shock("shock");
MCSDisplayInteger rain("rain");
MCSDisplayInteger indoor_light("indoor_light");
MCSDisplayInteger indoor_temp("indoor_temp");
MCSDisplayInteger indoor_hump("indoor_hump");

/*RFID函式*/
MFRC522 rfid(/*SS_PIN*/ RFID_pin, /*RST_PIN*/ UINT8_MAX);

/*溫溼度函式*/
DHT dht11_p15(Dht11_pin, DHT11);

/*讀取RFID*/
String mfrc522_readID()
{
  String ret;
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial())
  {
    MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

    for (byte i = 0; i < rfid.uid.size; i++) {
      ret += (rfid.uid.uidByte[i] < 0x10 ? "0" : "");
      ret += String(rfid.uid.uidByte[i], HEX);
    }
  }

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
  return ret;
}


//各個Task的時間，建議不用改
unsigned long intervals[] = {300, 2000, 2000, 2000, 2000};
unsigned long last[] = {0, 0, 0, 0, 0};

void setup() {
  Serial.begin(115200);
  pinMode(Fan_pin, OUTPUT);
  pinMode(Red_led_pin, OUTPUT);
  myservo.attach(SG90_servo_pin);
  lcd_i2c.begin(16, 2);
  pixels.begin();
  pixels.setBrightness(100);

  /*新增MCSLITE通道*/
  mcs.addChannel(on_off_ws2812);
  mcs.addChannel(on_off_fan);
  mcs.addChannel(switch_ws2812);
  mcs.addChannel(switch_fan);
  mcs.addChannel(read_RFID);
  mcs.addChannel(pir);
  mcs.addChannel(shock);
  mcs.addChannel(rain);
  mcs.addChannel(indoor_light);
  mcs.addChannel(indoor_temp);
  mcs.addChannel(indoor_hump);

  /*感測器腳位初始化*/
  digitalWrite(Fan_pin, LOW);
  digitalWrite(Red_led_pin, LOW);
  pinMode(WS2812_led_pin, OUTPUT);
  pinMode(Rain_sensor_pin, INPUT);
  pinMode(PIR_pin, INPUT);
  pinMode(Light_sensor_pin, INPUT);
  myservo.write(0);

  /*LCD顯示連線狀態*/
  lcd_i2c.setCursor(0, 0);
  lcd_i2c.print("Start Work.......");
  delay(3000);
  lcd_i2c.clear();
  lcd_i2c.setCursor(0, 0);
  lcd_i2c.print("CAVE_IOT_House");
  lcd_i2c.setCursor(0, 1);
  lcd_i2c.print("WiFi Connecting");
  while (WiFi.begin(_lwifi_ssid, _lwifi_pass) != WL_CONNECTED) {
    delay(1000);
  }
  Serial.println("WiFi連線成功");
  lcd_i2c.clear();
  lcd_i2c.setCursor(0, 0);
  lcd_i2c.print("WiFi Connected");
  while (!mcs.connected()) {
    mcs.connect();
  }
  Serial.println("MCSLite連線成功");
  lcd_i2c.setCursor(0, 1);
  lcd_i2c.print("MCSLite Connected");
  lcd_i2c.clear();
  lcd_i2c.setCursor(0, 0);
  lcd_i2c.print("MCSLite IP: ");
  lcd_i2c.setCursor(0, 1);
  lcd_i2c.print(_your_MCSLite_IP);
  delay(3000);

  Now_time = millis();

  /*感測器初始化*/
  SPI.begin();
  dht11_p15.begin();
  rfid.PCD_Init();
}

void loop() {
  /*感測器變數設定*/
  Var_rfid_string = mfrc522_readID();
  Var_light_sensor = (map(analogRead(Light_sensor_pin), 0, 4095, 0, 100));
  Var_shock = analogRead(Shocked_pin);
  Var_temp = dht11_p15.readTemperature();
  Var_humi = dht11_p15.readHumidity();
  Var_pir = digitalRead(PIR_pin);
  Var_rain = digitalRead(Rain_sensor_pin);

  while (!mcs.connected()) {
    mcs.connect();
    if (mcs.connected()) {
      Serial.println("MCS Reconnected.");
    }
  }
  mcs.process(100);

  firstTask(); //RFID及震動感測器偵測

  secondTask();//智慧風扇開關偵測

  thirdTask();//智慧電燈開關偵測

  fourthTask();//LCD螢幕顯示

  fifthTask();//上傳感測器數值到MCSLITE
}

