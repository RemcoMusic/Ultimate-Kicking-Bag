#include "Fastled.h"

class ledDriver{
    public:
        void setup();
        void clearLeds();
        void setLeds(String side);
        void sensorStatusLight(bool status);
    private:
        #define PIN 14
        #define NUM_LEDS 24
        void lightLeds(String mode);   
};