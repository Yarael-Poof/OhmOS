#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "ACS712.h"
#include <math.h>
#include <MemoryFree.h>
float c;
int fanPin = 4;
int alarmPin = 3;
LiquidCrystal_I2C lcd(0x27,20,4);
ACS712 sensor(ACS712_20A, A0);
unsigned long time;
long interval = 500;
int alarmState = LOW;  
long previousMillis = 0; 
char vNum[] = "170817v3.1";
char eCode[] = "0000";
int badStatus = 0;
void b();
void d();


double Thermistor(int RawADC) {
 double Temp;
 Temp = log(10000.0*((1024.0/RawADC-1)));
 Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
 Temp = Temp - 273.15;
 return Temp;
}

typedef void (* Caller)();
Caller FuncCall[] = {&a, &b, &d}; //initialize addresses for pointers 
//String func_list[] = {"a","b","c"}; //this is a terrible way to do this
char func_list[] = {'a', 'b', 'd'};   //use this instead - if you wish


void setup() {
  Serial.begin(9600);
  pinMode(fanPin, OUTPUT);
  pinMode(alarmPin, OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,1);
  lcd.print(String("OhmOS") + " " + (char)244);
  lcd.setCursor(0,2);
  lcd.print(vNum);
  lcd.setCursor(0,3);
  lcd.print(String("RAM free:") + freeMemory() + " " + "bytes");
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

  if (int(Thermistor(analogRead(2))) > 40 )
  {
    lcd.setCursor(0,3);
    digitalWrite(fanPin, HIGH);
    lcd.print(String("T :") + int(Thermistor(analogRead(2))) + char(223)+ "C");
    delay(300);
    lcd.setCursor(16,0);
    lcd.print("    ");
    badStatus = 1;
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
   badStatus = 0;
   char eCode[] = "0001";
   delay(300);
   
  }
  if(Serial.available())
        switch(Serial.read()){
            case 'a':           //this could also be func_list[0], if you want
                FuncCall[0]();
                break;

            case 'b':
                FuncCall[1]();
                break;

            case 'c':
                FuncCall[2]();
                break;

            default:
                Serial.println("There was an error\n");
                break;
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

void a(int badstatus, char eCode, char vNum)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Error State =" + badStatus);
  if(badStatus == 1)
  {
  lcd.setCursor(0,1);
  lcd.print("Error Code :" + eCode);
  }
  else
  {
  }
  lcd.setCursor(4,0);
  lcd.print("FW Version:"+vNum);
  delay(5000);
  return 0;





}
void b(){
    Serial.println("Called Function: b\n");
}

void d(){
    Serial.println("Called Function: c\n");
}


