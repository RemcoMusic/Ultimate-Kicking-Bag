#include <Arduino.h>
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "settings.h"

class webserver{  
    private:
        AsyncWebSocket * ws;
        AsyncWebServer * server;
    public:
        void startAsyncWebServer();
        void setTime(long reactionTime);
        void setSystemStatus(bool status);
};