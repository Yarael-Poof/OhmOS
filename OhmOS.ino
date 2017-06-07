#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "ACS712.h"
#include <math.h>
float c;
int fanPin = 4;
LiquidCrystal_I2C lcd(0x27,20,4);
ACS712 sensor(ACS712_20A, A0);

double Thermistor(int RawADC) {
 double Temp;
 Temp = log(10000.0*((1024.0/RawADC-1)));
 Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
 Temp = Temp - 273.15;
 return Temp;
}



void setup() {
  Serial.begin(9600);
  pinMode(fanPin, OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,2);
  lcd.print(String("OhmOS") + " " + (char)244);
  lcd.setCursor(0,3);
  lcd.print("210517v2");
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
  float voltage = (voltageSensor * 29.2) / 1024.0;
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
  lcd.setCursor(12,0);
  lcd.print("STS:");
  if (int(Thermistor(analogRead(2))) > 27)
  {
    lcd.setCursor(0,3);
    digitalWrite(fanPin, HIGH);
    lcd.print(String("T :") + int(Thermistor(analogRead(2))) + char(223)+ "C");
    delay(300);
    lcd.setCursor(16,0);
    lcd.print("    ");
    lcd.setCursor(16,0);
    lcd.print("TEMP");
  }
  else
  {
   lcd.setCursor(0,3);
   digitalWrite(fanPin, LOW);
   lcd.print(String("T :") + int(Thermistor(analogRead(2))) + char(223)+ "C");
   lcd.setCursor(16,0);
   lcd.print("    ");
   lcd.setCursor(16,0);
   lcd.print("OK");
   delay(300);
   
  }

}
