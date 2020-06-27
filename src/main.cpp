#include <Arduino.h>
#include "main.h"

MPU9250 gyro(Wire,0x68);
static int taskcore =1;

void makeTasks(){
    xTaskCreatePinnedToCore(
    gyroscope,
    "Gyroscope_readings",
    10000,
    &gyro,
    1,
    NULL,
    taskcore);

    xTaskCreate(
    asyncWebServer,
    "Async_WebServer",
    1000,
    NULL,
    2,
    NULL);
}

void initMPU(){ 
  int gyroStatus = gyro.begin();
  if(gyroStatus == 1){
    Serial.println("Gyroscope is initialized");
  } else {
    Serial.print("Gyroscope failed with error code: ");
    Serial.println(gyroStatus);
  }
}

void setup() {
  Serial.begin(115200);
  initMPU();

  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  Serial.print("Setting Access Point…");
  WiFi.softAP(wifiCredentials::ssid, wifiCredentials::password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  makeTasks();
}

void loop(){}