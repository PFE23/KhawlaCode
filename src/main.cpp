#include <Arduino.h>
#include "MyLiquidCrystal.hpp"



void setup(){
  Serial.begin(115200);
  lcd.welcomeMessage();
  lcd.updateValues(3, 4, "00:01:33", "00:05:33", "00:10:33"
  , "00:11:33");
}

void loop(){
}
/*#include <Arduino.h>

void setup(){
  Serial.begin(115200);
  Serial.print("Hello world");
}

void loop(){
  
}*/