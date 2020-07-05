#pragma once
#include <Arduino.h>

namespace wifiCredentials{
    extern const char* ssid;
    extern const char* password;
}

struct GlobalData {
        String gamemode = "left";
        int8_t difficulty = 10;
        bool triggerButton = false;
        String systemStatus = "unknown";
        };

       extern GlobalData globalData;

