#ifndef CYCLE_H
#define CYCLE_H
#include <Arduino.h>
#include <SecuredLinkedList.h>
#include <ctime>
#define PIN_LED           BUILTIN_LED
/**
 * @brief Handle the starting and ending time of a cycle.
 * 
 */
typedef struct{
  time_t starTime = 0;
  time_t endTime = 0;
} cycle_t;

typedef struct{
  cycle_t minCycle;
  cycle_t maxCycle;
  cycle_t currentCycle;
  time_t totalWorkingTime = 0;
  time_t totalSleepingTime = 0;
}stats_t;

stats_t stats;
SecuredLinkedList<cycle_t> *cycles = new SecuredLinkedList<cycle_t>();
bool isOnCycle = false;
bool bUpdateDisplay = false;

long cycleDuration(cycle_t c){
  long d = c.endTime - c.starTime;
  
  return (d < 0) ? 0 : d;
}

time_t totalPasTime(cycle_t firstC, cycle_t lastC, time_t workTime){
  return firstC.endTime - lastC.starTime - workTime;
}

void printCycle(cycle_t c){
  Serial.printf("[%d --> %d]", c.starTime, c.endTime);
}

// TODO make this function uninterruptible
void IRAM_ATTR startCycle(){
  if(!isOnCycle){
    stats.currentCycle.starTime = millis();
    isOnCycle = true;
    digitalWrite(PIN_LED, HIGH);
    bUpdateDisplay = true;
  }
}
// TODO make this function uninterruptible
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
    bUpdateDisplay = true;
  }
}

//char buffer[ 10 ];
typedef char hms[10];

char* convert2HMS(time_t delayMS){
  time_t timestamp = delayMS/1000;
  struct tm * pTime = localtime( & timestamp );
  static hms buffer;
  
  strftime( buffer, 10, "%H:%M:%S", pTime );

  return buffer;
}
void testPrintStats(){
  Serial.printf("%s;%d;%d\n",
  convert2HMS(stats.currentCycle.starTime), stats.currentCycle.starTime,
  cycles->size());
}

void printStats2(){
  hms starTime, endTime, currentCycle, minCycle, maxCycle, totalWorkingTime, totalSleepingTime;
  snprintf(starTime, 10, "%s", convert2HMS(stats.currentCycle.starTime));
  snprintf(endTime, 10, "%s", convert2HMS(stats.currentCycle.endTime));
  snprintf(currentCycle, 10, "%s", convert2HMS(cycleDuration(stats.currentCycle)));
  snprintf(minCycle, 10, "%s", convert2HMS(cycleDuration(stats.minCycle)));
  snprintf(maxCycle, 10, "%s", convert2HMS(cycleDuration(stats.maxCycle)));
  snprintf(totalWorkingTime, 10, "%s", convert2HMS(stats.totalWorkingTime));
  snprintf(totalSleepingTime, 10, "%s", convert2HMS(stats.totalSleepingTime));

  Serial.printf("%s;%s;%s;%s;%s;%s;%s;%d\n",
  starTime, endTime, currentCycle,
  minCycle, maxCycle, totalWorkingTime, totalSleepingTime,
  cycles->size());
}


void printStats(){
  Serial.printf("%d;%d;%d;%d;%d;%d;%d;%d\n",
  stats.currentCycle.starTime, stats.currentCycle.endTime, cycleDuration(stats.currentCycle),
  cycleDuration(stats.minCycle), cycleDuration(stats.maxCycle),
  stats.totalWorkingTime, stats.totalSleepingTime, cycles->size());
}

#endif