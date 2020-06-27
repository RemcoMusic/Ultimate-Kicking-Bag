#include <Arduino.h> 
#include "hit_detection.h"
#include "webserver.h"
#include "ledDriver.h"
#include "main.h"

webserver webServer;
ledDriver ledRing;

void asyncWebServer(void * parameter){
  webServer.startAsyncWebServer();
  for(;;){
    vTaskDelay(40);
  }
}

void gyroscope(void * paramater){
  HitDetection h;
  h.startSensor((MPU9250*)paramater);
  ledRing.setup();

  //Variables for start time
  long previousTriggerMillis = 0;
  unsigned long currentTriggerMillis = 0;
  long interval = 0;

  //Variables for measering hit time
  unsigned long currentStartTime = 0;
  unsigned long currentEndTime= 0;
  long elapsedTime = 0;

  //Variables for calibrating sensor values
  float calibratedMedianValue = 0;
  float tempMedianCalculation = 0;

  for (int i = 0; i < 100; i++){
    tempMedianCalculation += h.readSensor();
  }

  calibratedMedianValue = abs(tempMedianCalculation/100.00);
  
  Serial.print("Calibrated value: ");
  Serial.println(calibratedMedianValue);

  //Variables for resetting the process
  bool kicked = true;
  bool timer = false;
  
  for(;;){
    if(globalData.triggerButton){
      timer = true;
      previousTriggerMillis = millis();
      interval = random(2000,6000);
      globalData.triggerButton = false;
    }

    currentTriggerMillis = millis();

    if(timer){
      if (currentTriggerMillis - previousTriggerMillis >= interval){
        previousTriggerMillis = currentTriggerMillis;
        timer = false;
        kicked = false;
        ledRing.setLeds(globalData.gamemode);
        currentStartTime = millis();
      }
    }

    while(!kicked){
      float tempReading = abs(h.readSensor());
      Serial.println(tempReading);

        if((tempReading - calibratedMedianValue) > globalData.difficulty){
          currentEndTime = millis();
          elapsedTime = currentEndTime - currentStartTime;

          webServer.setTime(elapsedTime);
          ledRing.clearLeds();
          kicked = true;
        }
    }
 
    TIMERG0.wdt_wprotect=TIMG_WDT_WKEY_VALUE;
    TIMERG0.wdt_feed=1;
    TIMERG0.wdt_wprotect=0;
  }
}