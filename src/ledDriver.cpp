#include <Arduino.h>
#include "ledDriver.h"

CRGB leds[NUM_LEDS];

void ledDriver::setup(){
    LEDS.addLeds<WS2812,PIN,GRB>(leds,NUM_LEDS);
    clearLeds();
}

void ledDriver::setLeds(String side){
    if(side == "left"){
        lightLeds("left");
    }
    if(side == "right"){
        lightLeds("right");
    }
    if(side == "random"){
        int randomChoice = random(1,3);
        if(randomChoice == 1){
            lightLeds("left");
        } else if (randomChoice == 2){
            lightLeds("right");
        }
    }
    FastLED.setBrightness(50);
    FastLED.show();
}

void ledDriver::lightLeds(String mode){
    if(mode == "left"){
        for (int i = 13; i < 24; i++){
            leds[i] = CRGB::Green;
        }
    }
    if(mode == "right"){
        for (int i = 1; i < 13; i++){
            leds[i] = CRGB::Green;
        }
    }
}

void ledDriver::clearLeds()
{
	FastLED.clear();
	FastLED.setBrightness(0);
	FastLED.show();
}