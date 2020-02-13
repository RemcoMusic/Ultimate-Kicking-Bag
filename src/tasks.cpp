#include <Arduino.h>
#include <ESPmDNS.h>
#include "RemoteDebug.h"
#include "OTA.h"
#include "tasks.h"
#include "hit_detection.h"

RemoteDebug Debug;
OTA OTAupdate;
HitDetection sensor;

void processCmdRemoteDebug() {

	String lastCmd = Debug.getLastCommand();

	if (lastCmd == "ota") {
      debugI("OTA is starting");
      OTAupdate.startOTA();
  }

	if (lastCmd == "test") {
      debugI("Input received test succeded");
  }
}

Tasks::Tasks()
{
  xTaskCreate(
    remoteDebugger,
    "Remote_Debugger",
    10000,
    NULL,
    1,
    NULL);

    xTaskCreate(
    gyroscope,
    "Gyroscope_readings",
    10000,
    NULL,
    1,
    NULL);
}

void Tasks::gyroscope(void * paramater) {
  sensor.startSensor();
  for(;;){
    Serial.println(sensor.readSensor());
    vTaskDelay(40);
  }
}

void Tasks::remoteDebugger(void * parameter) {

    if (MDNS.begin("TaekwondoDebug")) {
        Serial.print("* MDNS responder started. Hostname -> ");
        Serial.println("TaekwondoDebug");
    }

    MDNS.addService("telnet", "tcp", 23);

    Debug.begin("TaekwondoDebug");        // Initiaze the telnet server
    Debug.setResetCmdEnabled(true);       // Enable the reset command
	  Debug.showProfiler(false);            // Profiler (Good to measure times, to optimize codes)
	  Debug.showColors(true);               // Colors
    Debug.setCallBackProjectCmds(&processCmdRemoteDebug);
  
    for(;;) {
      Debug.handle();   
    }
}