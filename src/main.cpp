#define PIN_LED           BUILTIN_LED
#include <Arduino.h>
#include "Button.hpp"
#include "Cycle.hpp"
#include <WiFi.h>
#include <WebServer.h>
#include <WebSockets.h>
#include "myWebPage.hpp"


#define PIN_START_BUTTON  23
#define PIN_STOP_BUTTON   22
#define ssid              "Redmi Note 11"
#define password          "isetbeja"
#define WAITTIME          2000

Button start(PIN_START_BUTTON);
Button stop(PIN_STOP_BUTTON);

WebServer server(80);

time_t startTime;

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

  startTime = millis();
  Serial.printf("start;stop;dur;shortD;longD;work;sleep;size\n");
  //Serial.printf("start;start;stop;stop;dur;dur;shortD;shortD;longD;longD;work;work;sleep;sleep;size\n");
}

void loop(){
  server.handleClient();
  time_t now = millis();
  if (now - startTime > WAITTIME){
    printStats2();
    startTime = now;
  }
}