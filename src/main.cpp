#include <Arduino.h>
#include "Button.hpp"
#include <SecuredLinkedList.h>
#include "Cycle.hpp"
#include <ctime>

#define PIN_START_BUTTON  23
#define PIN_STOP_BUTTON   22
#define PIN_LED           BUILTIN_LED

Button start(PIN_START_BUTTON);
Button stop(PIN_STOP_BUTTON);
bool isOnCycle = false;

typedef struct{
  cycle_t minCycle;
  cycle_t maxCycle;
  cycle_t currentCycle;
  unsigned long totalWorkingTime = 0;
  unsigned long totalSleepingTime = 0;
}stats_t;

stats_t stats;
// cycle_t c;
SecuredLinkedList<cycle_t> *cycles = new SecuredLinkedList<cycle_t>();
unsigned long elapsedTime = 0;

void IRAM_ATTR startCycle(){
  if(!isOnCycle){
    stats.currentCycle.starTime = millis();
    isOnCycle = true;
    digitalWrite(PIN_LED, HIGH);
  }
}

void IRAM_ATTR stopCycle(){
  if(isOnCycle){
    stats.currentCycle.endTime = millis();
    isOnCycle = false;
    stats.totalWorkingTime += cycleDuration(stats.currentCycle);
    digitalWrite(PIN_LED, LOW);
    cycles->unshift(stats.currentCycle);
    if(cycles->size() < 2){
      stats.maxCycle = stats.currentCycle;
      stats.minCycle = stats.currentCycle;
    }else{
      if(cycleDuration(stats.currentCycle) > cycleDuration(stats.maxCycle)){
        stats.maxCycle = stats.currentCycle;
      }
      if(cycleDuration(stats.currentCycle) < cycleDuration(stats.minCycle)){
        stats.minCycle = stats.currentCycle;
      }
      stats.totalSleepingTime = totalPasTime(stats.currentCycle, cycles->get(cycles->size()-1), 
                                stats.totalWorkingTime);/**/
    }
  }
}

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
  /*if(cycles->size() > 2){//false
    cycle_t cc;
    cc = cycles->get(0);
    printCycle(cc);
  }
  Serial.printf("%d;%d;%d;%d;%d bytes;%d bytes\n", c.starTime, c.endTime, 
          elapsedTime, sizeof(SecuredLinkedList<cycle_t>), cycles->size(),
          sizeof(SecuredLinkedList<cycle_t>)*cycles->size());*/
  delay(2000);
}