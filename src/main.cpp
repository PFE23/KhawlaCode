#define PIN_LED           BUILTIN_LED
#include <Arduino.h>
#include "Button.hpp"
#include "Cycle.hpp"
#include <ctime>

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
}

void loop(){
  Serial.printf("%d;%d;%d;%d;%d;%d;%d;%d\n",
  stats.currentCycle.starTime, stats.currentCycle.endTime, cycleDuration(stats.currentCycle),
  cycleDuration(stats.minCycle), cycleDuration(stats.maxCycle),
  stats.totalWorkingTime, stats.totalSleepingTime, cycles->size());
  delay(2000);
}