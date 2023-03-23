#ifndef CYCLE_H
#define CYCLE_H
#include <Arduino.h>
/**
 * @brief Handle the starting and ending time of a cycle.
 * 
 */
typedef struct{
  unsigned long starTime = 0;
  unsigned long endTime = 0;
} cycle_t;

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
#endif