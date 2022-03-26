#pragma once
#include <Arduino.h>
#include "ESPAsyncWebServer.h"
#include "ArduinoJson.h"
#include "SPIFFS.h"
#include "settings.h"

class webserver{  
    private:
        AsyncWebSocket * ws;
        AsyncWebServer * server;
    public:
        void startAsyncWebServer();
        void updateFrontEnd();
};