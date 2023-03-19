#include <Arduino.h>
#include "Button.cpp"

#define PIN_START_BUTTON  23
#define PIN_STOP_BUTTON   22
#define PIN_LED           13

Button start(PIN_START_BUTTON);
Button stop(PIN_STOP_BUTTON);
bool isOnCycle = false;
unsigned long starTime = 0;
unsigned long endTime = 0;
unsigned long elapsedTime = 0;

void IRAM_ATTR startCycle(){
  if(!isOnCycle){
    starTime = millis();
    isOnCycle = true;
  }
}

void IRAM_ATTR stopCycle(){
  if(isOnCycle){
    endTime = millis();
    isOnCycle = false;
    elapsedTime = endTime - starTime;
  }
}

void setup(){
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");
  start.begin(&startCycle);
  stop.begin(&stopCycle);
}

void loop(){
  //start.printStatus();
  Serial.printf("%d\t\t%d\t\t%d\n", starTime, endTime, elapsedTime);
  delay(1000);
}