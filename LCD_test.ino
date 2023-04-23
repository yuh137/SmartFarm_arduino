#include "DHT.h"
#include "MUX74HC4067.h"
#include "config.h"
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 02
#define DHTTYPE DHT11
#define ANALOG_PIN A0

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
MUX74HC4067 mux(0, 14, 12, 13, 15);
Servo SG90;

int h;
float t;
int l;
int rain_level;
int soil_h;
String msg = "";

AdafruitIO_Feed *humid_feed = io.feed("YOLOFARM_humidity");
AdafruitIO_Feed *temp_feed = io.feed("YOLOFARM_temperature");
AdafruitIO_Feed *light_feed = io.feed("YOLOFARM_lightlevel");
AdafruitIO_Feed *rain_feed = io.feed("YOLOFARM_rain");
AdafruitIO_Feed *soil_feed = io.feed("YOLOFARM_soil");

//unsigned long prevMillis = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  mux.signalPin(ANALOG_PIN, INPUT, ANALOG);
  lcd.begin();
  lcd.backlight();
  dht.begin();
  io.connect();
}

void loop() {
  // put your main code here, to run repeatedly:
  io.run();
  h = dht.readHumidity();
  t = dht.readTemperature();
  l = 1024 - mux.read(0);
  soil_h = 1024 - mux.read(1);
  humid_feed->save(h);
  temp_feed->save(t);
  light_feed->save(l);
  soil_feed->save(soil_h);
//  rain_feed->save(rain_level);
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("YOLOFARM");
  msg = "Temp: " + (String)t + "*C" + " - Humidity: " + (String)h + "% - Light level: " + (String)l + "lux - Soil humidity: " + (String)soil_h;
  scrollMessage(msg, 250);
  delay(1000);
}

void scrollMessage(String msg, int delayTime) {
  String temp = " " + msg;
  for (int position = 0; position < msg.length(); position++) {
    lcd.setCursor(0, 1);
    lcd.print(msg.substring(position, position + 16));
    if (position > msg.length() - 16){
      lcd.print(temp.substring(0, position - msg.length() + 16));
    }
    delay(delayTime);
  }
}
