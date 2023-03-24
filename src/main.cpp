#define PIN_LED           BUILTIN_LED
#include <Arduino.h>
#include "Button.hpp"
#include "Cycle.hpp"


#define PIN_START_BUTTON  23
#define PIN_STOP_BUTTON   22

Button start(PIN_START_BUTTON);
Button stop(PIN_STOP_BUTTON);

void setup(){
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");
  pinMode(PIN_LED, OUTPUT);
  start.begin(&startCycle);
  stop.begin(&stopCycle);
  cycles->clear();
  Serial.printf("The size of stats is %d bytes\n", sizeof(stats_t));
  Serial.printf("start;stop;dur;shortD;longD;work;sleep;size\n");
  //Serial.printf("start;start;stop;stop;dur;dur;shortD;shortD;longD;longD;work;work;sleep;sleep;size\n");
}

void loop(){
  printStats2();
  delay(1000);
}