#include <Wire.h>

#include <LiquidCrystal.h>
#include <Power.h>
#include <Sonar.h>
#include <Motors.h>
#include <Rcr.h>

/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led1 = 32;
int led2 = 31;

int VCC = 42;

LiquidCrystal lcd(15, 16, 17, 18, 19, 20);

uint16_t c1, c2, vcc;
int sb;

//MotorsCurrent MotorsCMeasure();

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led1, OUTPUT);     
  pinMode(led2, OUTPUT);
  
  lcd.begin(20,4);
  lcd.print("hello world!");
  delay(1000);
  lcd.clear();
  
  LeftMotors.setSpeed(120);
  LeftMotors.setDirection(MDIR_FORWARD);
  LeftMotors.start();

  RightMotors.setSpeed(120);
  RightMotors.setDirection(MDIR_FORWARD);
  RightMotors.start();
  
  AdcMeasure.startConversion();

  digitalWrite(led2, HIGH); 
   
  Sonar1.setThreshold(200);
  Sonar2.setThreshold(200);
  Sonar3.setThreshold(200);
  
  Sonar1.setSens(8);
  Sonar2.setSens(8);
  Sonar3.setSens(-8);
  Sonar3.setRange(500);
  //RadioUsart.setSpeed(40);
}

// the loop routine runs over and over again forever:
void loop() {
  
  c1 = MotorsCMeasure.getCurrent(1);
  c2 = MotorsCMeasure.getCurrent(2);
  if (c1 > 300 || c2 > 600)
    digitalWrite(led1, HIGH);
  else
    digitalWrite(led1, LOW);
  
  lcd.setCursor(0,0);
  lcd.print(c1);
  lcd.setCursor(6,0);
  lcd.print(c2);
  
  lcd.setCursor(0,1);
  lcd.print(VBat.getVoltage());


 
  Serial1.println(Sonar3.getDistance());
  
  delay(150);               // wait for a second
  lcd.clear();
}
