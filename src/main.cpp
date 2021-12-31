#include <Arduino.h>
#include "main.h"

SPIClass BussSelection(3);
MPU9250 gyro(BussSelection, 5);

static int taskcore = 1;

void makeTasks(){
    // xTaskCreatePinnedToCore(
    // gyroscope,
    // "Gyroscope_readings",
    // 1600,
    // &gyro,
    // 1,
    // NULL,
    // taskcore);

    xTaskCreate(
    asyncWebServer,
    "Async_WebServer",
    1600,
    NULL,
    2,
    NULL);

    // xTaskCreate(
    // runGame,
    // "Run_game",
    // 1600,
    // NULL,
    // 3,
    // NULL);
}

void initMPU(){
  int gyroStatus = 0;

  do
  {
    gyroStatus = gyro.begin();
    delay(2000);
  } while (gyroStatus != 1);
  globalData.sensorWorking = true;
}

void setup() {
  Serial.begin(115200);
  // initMPU();

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