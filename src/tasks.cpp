#include <Arduino.h> 
#include "hit_detection.h"
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
  HitDetection h;

  if(h.startSensor((MPU9250*)paramater)){
    ledRing.sensorStatusLight(true);
  } else {
    ledRing.sensorStatusLight(false);
  }
  
  h.calibrateMedianValue();
 
  for(;;){
    if(globalData.enableSensor){
      globalData.sensorValue = abs(h.readSensor());
    } else if(globalData.gamemode == "printAccel"){
      h.printAccelValues();
    } else if(globalData.gamemode == "printGyro"){
      h.printGyroValues();
    }
    vTaskDelay(40);
  }
}