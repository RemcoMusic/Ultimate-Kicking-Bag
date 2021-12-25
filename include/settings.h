#pragma once
#include <Arduino.h>

namespace wifiCredentials{
    extern const char* ssid;
    extern const char* password;
}

struct GlobalData {
        String gamemode = "fast";
        String ledSide = "left";
        int8_t difficulty = 10;
        bool triggerButton = false;
        bool sensorWorking = false;
        bool enableSensor = false;
        float sensorValue = 0.0;
        float calibratedMedianValue = 0.0;
        String systemStatus = "unknown";
        };

       extern GlobalData globalData;

