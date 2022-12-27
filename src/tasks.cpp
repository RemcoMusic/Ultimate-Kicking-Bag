#include <Arduino.h> 
#include "read_sensor.h"
#include "webserver.h"
#include "ledDriver.h"
#include "gamemodes.h"
#include "main.h"

webserver webServer;
ledDriver ledRing;
gamemodes gameMode;

void asyncWebServer(void * parameter){
  webServer.startAsyncWebServer();
  for(;;){
    vTaskDelay(40);
  }
}

void runGame(void * parameter){
  ledRing.setup();

  for(;;){
    if(globalData.triggerButton){
      if(globalData.gamemode == "fast"){
        gameMode.fastestKick(ledRing, webServer);
      } else if(globalData.gamemode == "amount"){
        gameMode.fastestAmountOfKicks(ledRing, webServer);
      }
    }
    vTaskDelay(40);
  }
}

void gyroscope(void * paramater){
  ReadSensor sensor;

  if(sensor.start((MPU9250*)paramater)){
    ledRing.sensorStatusLight(true);
  } else {
    ledRing.sensorStatusLight(false);
  }
  
  sensor.calibrateMedianValue("x");
 
  for(;;){
    if(globalData.enableSensor){
      globalData.sensorValue = abs(sensor.readAccellerometer("x"));
    } else if(globalData.gamemode == "printAccel"){
      sensor.printAccelValues();
    } else if(globalData.gamemode == "printGyro"){
      sensor.printGyroValues();
    }
    vTaskDelay(40);
  }
}