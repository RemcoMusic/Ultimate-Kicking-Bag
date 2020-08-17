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
  
  //Variables for calibrating sensor values
  float tempMedianCalculation = 0;

  for (int i = 0; i < 100; i++){
    tempMedianCalculation += h.readSensor();
  }

  globalData.calibratedMedianValue = abs(tempMedianCalculation/100.00);
  
  Serial.print("Calibrated value: ");
  Serial.println(globalData.calibratedMedianValue);

  for(;;){
    if(globalData.enableSensor){
      globalData.sensorValue = abs(h.readSensor());
      Serial.println(globalData.sensorValue);
    }

    TIMERG0.wdt_wprotect=TIMG_WDT_WKEY_VALUE;
    TIMERG0.wdt_feed=1;
    TIMERG0.wdt_wprotect=0;
  }
}