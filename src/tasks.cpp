#include <Arduino.h> 
#include "hit_detection.h"
#include "webserver.h"
#include "ledDriver.h"
#include "main.h"

webserver webServer;
ledDriver ledRing;

bool triggerAction = false;
String lightModes;

void asyncWebServer(void * parameter){
  webServer.startAsyncWebServer();
  for(;;){
    triggerAction = webServer.readTriggerButton();
    lightModes = webServer.getMode();
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

    if(triggerAction){
      timer = true;
      previousTriggerMillis = millis();
      interval = random(2000,7000);
      triggerAction = false;
      webServer.resetTriggerButton();
    }


    currentTriggerMillis = millis();

    if(timer){
      if (currentTriggerMillis - previousTriggerMillis >= interval){
        previousTriggerMillis = currentTriggerMillis;
        timer = false;
        kicked = false;
        ledRing.setLeds(lightModes);
        currentStartTime = millis();
      }
    }

    while(!kicked){
      float tempReading = abs(h.readSensor());

        if((tempReading - calibratedMedianValue) > 10){
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