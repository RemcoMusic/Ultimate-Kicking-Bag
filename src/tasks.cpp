#include <Arduino.h>
#include "RemoteDebug.h" 
#include <ESPmDNS.h>
#include "OTA.h"
#include "hit_detection.h"
#include "webserver.h"
#include "ledDriver.h"
#include "main.h"

RemoteDebug Debug;
webserver webServer;
OTA OTAupdate;
ledDriver ledRing;

bool triggerAction = false;

void processCmdRemoteDebug(){

	String lastCmd = Debug.getLastCommand();

	if (lastCmd == "ota") {
      debugI("OTA is starting");
      OTAupdate.startOTA();
  }

	if (lastCmd == "test") {
      debugI("Input received test succeded");
  }
}

void remoteDebugger(void * parameter){

    if (MDNS.begin("TaekwondoDebug")){
        Serial.print("* MDNS responder started. Hostname -> ");
        Serial.println("TaekwondoDebug");
    }

    MDNS.addService("telnet", "tcp", 23);

    Debug.begin("TaekwondoDebug");        // Initiaze the telnet server
    Debug.setResetCmdEnabled(true);       // Enable the reset command
	  Debug.showProfiler(false);            // Profiler (Good to measure times, to optimize codes)
	  Debug.showColors(true);               // Colors
    Debug.setCallBackProjectCmds(&processCmdRemoteDebug);
  
    for (;;){
      Debug.handle();   
    }
}

void asyncWebServer(void * parameter){
  webServer.startAsyncWebServer();
  for(;;){
    triggerAction = webServer.readTriggerButton();
    vTaskDelay(40);
  }
}

void gyroscope(void * paramater){
  HitDetection h;
  h.startSensor((MPU9250*)paramater);
  ledRing.setup();

  long previousTriggerMillis = 0;
  unsigned long currentTriggerMillis = 0;
  long interval = 0;

  unsigned long currentStartTime = 0;
  unsigned long currentEndTime= 0;
  long elapsedTime = 0;

  bool resetVariables = true;
  bool kicked = false;

  for(;;){
    if(triggerAction){
      if(resetVariables){
        previousTriggerMillis = 0;
        interval = random(3000,7000);

        currentStartTime = 0;
        currentEndTime = 0;
        elapsedTime = 0;
        kicked = false;
      }

      currentTriggerMillis = millis();
      if(currentTriggerMillis - previousTriggerMillis > interval){

        resetVariables = false;

        ledRing.setLeds("left");
        currentStartTime = millis();

        while(!kicked){
          float tempReading = h.readSensor();

          if(tempReading > 16){

            currentEndTime = millis();
            elapsedTime = currentEndTime - currentStartTime;

            webServer.setTime(elapsedTime);
            webServer.resetTriggerButton();
            ledRing.clearLeds();

            kicked = true;
            resetVariables = true;
            triggerAction = false;
          }
        }
        kicked = false;
      }
    }
    
    TIMERG0.wdt_wprotect=TIMG_WDT_WKEY_VALUE;
    TIMERG0.wdt_feed=1;
    TIMERG0.wdt_wprotect=0;
  }
}