#include <Arduino.h>
#include "WiFi.h"
#include "SPIFFS.h"
#include "settings.h"
#include "MPU9250.h"
#include "main.h"

MPU9250 gyro(Wire,0x68);

void makeTasks(){
   xTaskCreate(
    remoteDebugger,
    "Remote_Debugger_task",
    10000,
    NULL,
    3,
    NULL);

    xTaskCreate(
    gyroscope,
    "Gyroscope_readings",
    10000,
    &gyro,
    1,
    NULL);

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
    int calibratedStatus = gyro.calibrateAccel();
    if(gyroStatus == 1){
      Serial.println("Gyroscope is initialized");
    } else {
      Serial.print("Gyroscope failed with error code: ");
      Serial.println(gyroStatus);
    }
    if(calibratedStatus == 1){
      Serial.println("Accelarometer is calibrated");
    } else {
      Serial.print("Accelarometer failed to calibrate: ");
      Serial.println(calibratedStatus);
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