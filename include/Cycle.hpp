#ifndef CYCLE_H
#define CYCLE_H
#include <Arduino.h>
#include <SecuredLinkedList.h>
/**
 * @brief Handle the starting and ending time of a cycle.
 * 
 */
typedef struct{
  unsigned long starTime = 0;
  unsigned long endTime = 0;
} cycle_t;

typedef struct{
  cycle_t minCycle;
  cycle_t maxCycle;
  cycle_t currentCycle;
  unsigned long totalWorkingTime = 0;
  unsigned long totalSleepingTime = 0;
}stats_t;

stats_t stats;
SecuredLinkedList<cycle_t> *cycles = new SecuredLinkedList<cycle_t>();
bool isOnCycle = false;

unsigned long cycleDuration(cycle_t c){
  unsigned long d = c.endTime - c.starTime;
  if(d < 0) return 0;
  return d;
  //return (d < 0) ? 0 : d;
}

unsigned long totalPasTime(cycle_t firstC, cycle_t lastC, unsigned long workTime){
  return firstC.endTime - lastC.starTime - workTime;
}

void printCycle(cycle_t c){
  Serial.printf("[%d --> %d]", c.starTime, c.endTime);
}

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

void printStats(){
  Serial.printf("%d;%d;%d;%d;%d;%d;%d;%d\n",
  stats.currentCycle.starTime, stats.currentCycle.endTime, cycleDuration(stats.currentCycle),
  cycleDuration(stats.minCycle), cycleDuration(stats.maxCycle),
  stats.totalWorkingTime, stats.totalSleepingTime, cycles->size());
}

#endif