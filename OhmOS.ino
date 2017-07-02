#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "ACS712.h"
#include <math.h>
float c;
int fanPin = 4;
int alarmPin = 3;
LiquidCrystal_I2C lcd(0x27,20,4);
ACS712 sensor(ACS712_20A, A0);
unsigned long time;
long interval = 500;
int alarmState = LOW;  
long previousMillis = 0; 
char vNum[] = "020717v3";

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
  pinMode(alarmPin, OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,2);
  lcd.print(String("OhmOS") + " " + (char)244);
  lcd.setCursor(0,3);
  lcd.print(vNum);
  delay(2000);
  lcd.clear();
  lcd.print("Calibrating... ");
  int zero = sensor.calibrate();
  delay(1000);
  lcd.setCursor(0,1);
  lcd.print("Done!");
  digitalWrite(alarmPin, HIGH);
  delay(100);
  digitalWrite(alarmPin, LOW);
  delay(100);
  digitalWrite(alarmPin, HIGH);
  delay(100);
  digitalWrite(alarmPin, LOW);
  delay(400);
  lcd.clear();
  
}

void loop() {
  int voltageSensor = analogRead(A1);
  float I = sensor.getCurrentDC();
  c = -1 * I;
  float voltage = ((voltageSensor * 28.3) / 1023.0) + 0.2;
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
  lcd.setCursor(12,1);
  lcd.print("CC-INOP");

  if (int(Thermistor(analogRead(2))) > 45 )
  {
    lcd.setCursor(0,3);
    digitalWrite(fanPin, HIGH);
    lcd.print(String("T :") + int(Thermistor(analogRead(2))) + char(223)+ "C");
    delay(300);
    lcd.setCursor(16,0);
    lcd.print("    ");
    lcd.setCursor(16,0);
    lcd.print("TEMP");
    alarmSequence();
  }
  else
  {
   lcd.setCursor(0,3);
   digitalWrite(fanPin, LOW);
   digitalWrite(alarmPin, LOW);
   lcd.print(String("T :") + int(Thermistor(analogRead(2))) + char(223)+ "C");
   lcd.setCursor(16,0);
   lcd.print("    ");
   lcd.setCursor(16,0);
   lcd.print("OK");
   delay(300);
   
  }

}

void alarmSequence()
{
 unsigned long currentMillis = millis();
 if(currentMillis - previousMillis > interval) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;   
 
    // if the LED is off turn it on and vice-versa:
    if (alarmState == LOW)
      alarmState = HIGH;
    else
      alarmState = LOW;
 
    // set the LED with the ledState of the variable:
    digitalWrite(alarmPin, alarmState);
  }
}



