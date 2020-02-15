#include <Arduino.h>
#include "ledDriver.h"

CRGB leds[NUM_LEDS];

void ledDriver::setup(){
    LEDS.addLeds<WS2812,PIN,GRB>(leds,NUM_LEDS);
    clearLeds();
}

void ledDriver::setLeds(String side){
    if(side == "left"){
        for (int i = 0; i < 5; i++){
            leds[i] = CRGB::Green;
        }
        for (int i = 23; i > 19; i--){
            leds[i] = CRGB::Green;
        }
    }
    if(side == "right"){
        for (int i = 9; i < 15; i++){
            leds[i] = CRGB::Green;
        }
    }
    FastLED.setBrightness(50);
    FastLED.show();
}

void ledDriver::clearLeds()
{
	FastLED.clear();
	FastLED.setBrightness(0);
	FastLED.show();
}