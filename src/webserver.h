#include <Arduino.h>
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

class webserver{  
    private:
        //void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len);
    public:
        void startAsyncWebServer();
        bool readTriggerButton();
        void resetTriggerButton();
        void setTime(long reactionTime);
        String getMode();
};