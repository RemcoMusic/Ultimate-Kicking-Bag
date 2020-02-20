#include <Arduino.h>

class webserver{   
    public:
        void startAsyncWebServer();
        bool readTriggerButton();
        void resetTriggerButton();
        void setTime(long reactionTime);
        String getMode();
};