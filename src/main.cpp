#include <Arduino.h>
#include "WiFi.h"
#include "SPIFFS.h"
#include "tasks.h"

const char* ssid = "Taekwondo-Papendrecht";
const char* password = "Test1234";

void setup() {
  Serial.begin(115200);
  
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  Serial.print("Setting Access Point…");
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  Tasks initTasks;
}

void loop(){
  delay(500);
}