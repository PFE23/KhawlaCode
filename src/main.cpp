#define PIN_LED           BUILTIN_LED
#include <Arduino.h>
#include "Button.hpp"
#include "Cycle.hpp"
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h> 
#include "myWebPage.hpp"


#define PIN_START_BUTTON  23
#define PIN_STOP_BUTTON   22
#define ssid              "Redmi Note 11"
#define password          "isetbeja"
#define WAITTIME          2000

Button start(PIN_START_BUTTON);
Button stop(PIN_STOP_BUTTON);

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

time_t startTime;

void printStatsJ(){
  hms starTime, endTime, currentCycle, minCycle, maxCycle, totalWorkingTime, totalSleepingTime;
  String jsonString = "";
  StaticJsonDocument<200> doc;
  //webSocket.broadcastTXT(char_array);
  snprintf(starTime, 10, "%s", convert2HMS(stats.currentCycle.starTime));
  snprintf(endTime, 10, "%s", convert2HMS(stats.currentCycle.endTime));
  snprintf(currentCycle, 10, "%s", convert2HMS(cycleDuration(stats.currentCycle)));
  snprintf(minCycle, 10, "%s", convert2HMS(cycleDuration(stats.minCycle)));
  snprintf(maxCycle, 10, "%s", convert2HMS(cycleDuration(stats.maxCycle)));
  snprintf(totalWorkingTime, 10, "%s", convert2HMS(stats.totalWorkingTime));
  snprintf(totalSleepingTime, 10, "%s", convert2HMS(stats.totalSleepingTime));
  JsonObject object = doc.to<JsonObject>();
  object["nbCycles"] = cycles->size();         // create a JSON Object
  object["starTime"] = starTime;
  object["endTime"] = endTime;
  object["currentCycle"] = currentCycle;                    
  object["minCycle"] = minCycle;
  object["maxCycle"] = maxCycle;
  object["totalWorkingTime"] = totalWorkingTime;                    
  object["totalSleepingTime"] = totalSleepingTime;
  serializeJson(doc, jsonString);                   // convert JSON object to string
  //Serial.println(jsonString);                       // print JSON string to console for debug purposes (you can comment this out)
  webSocket.broadcastTXT(jsonString);


  /*snprintf(starTime, 10, "%s", convert2HMS(stats.currentCycle.starTime));
  snprintf(endTime, 10, "%s", convert2HMS(stats.currentCycle.endTime));
  snprintf(currentCycle, 10, "%s", convert2HMS(cycleDuration(stats.currentCycle)));
  snprintf(minCycle, 10, "%s", convert2HMS(cycleDuration(stats.minCycle)));
  snprintf(maxCycle, 10, "%s", convert2HMS(cycleDuration(stats.maxCycle)));
  snprintf(totalWorkingTime, 10, "%s", convert2HMS(stats.totalWorkingTime));
  snprintf(totalSleepingTime, 10, "%s", convert2HMS(stats.totalSleepingTime));

  Serial.printf("%s;%s;%s;%s;%s;%s;%s;%d\n",
  starTime, endTime, currentCycle,
  minCycle, maxCycle, totalWorkingTime, totalSleepingTime,
  cycles->size());*/
}

void setup(){
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");
  pinMode(PIN_LED, OUTPUT);
  start.begin(&startCycle);
  stop.begin(&stopCycle);
  cycles->clear();
  Serial.printf("The size of stats is %d bytes\n", sizeof(stats_t));
  // Connecting to WiFi
  Serial.print("Connecting to WiFi ");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(2000);
  }
  Serial.print(WiFi.localIP());
  server.on("/", []() {
    server.send(200, "text/html", myPage);});
  server.begin();
  webSocket.begin();

  startTime = millis();
  Serial.printf("start;stop;dur;shortD;longD;work;sleep;size\n");
  //Serial.printf("start;start;stop;stop;dur;dur;shortD;shortD;longD;longD;work;work;sleep;sleep;size\n");
}


void loop(){
  server.handleClient();
  webSocket.loop();
  time_t now = millis();
  if (now - startTime > WAITTIME){
    printStats2();
    printStatsJ();
    startTime = now;
  }
}