#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "ACS712.h"
float c;

LiquidCrystal_I2C lcd(0x27,20,4);
ACS712 sensor(ACS712_20A, A0);


void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,2);
  lcd.print(String("OhmOS") + " " + (char)244);
  lcd.setCursor(0,3);
  lcd.print("150517v1.1");
  delay(2000);
  lcd.clear();
  lcd.print("Calibrating... ");
  int zero = sensor.calibrate();
  delay(1000);
  lcd.setCursor(0,1);
  lcd.print("Done!");
  delay(500);
  lcd.clear();
 
}

void loop() {
  int voltageSensor = analogRead(A1);
  float I = sensor.getCurrentDC();
  c = -1 * I;
  float voltage = (voltageSensor * 28) / 1024.0;
  float power = voltage * c;
  float R = voltage / c;
  lcd.setCursor(0,0);
  lcd.print(String("I : ") + c);
  lcd.setCursor(10,0);
  lcd.print("A");
  lcd.setCursor(0,1);
  lcd.print(String("V :") + voltage);
  lcd.setCursor(10,1);
  lcd.print("V");
  lcd.setCursor(0,2);
  lcd.print(String("P :") + power);
  lcd.setCursor(10,2);
  lcd.print("W");
  delay(300);
}
