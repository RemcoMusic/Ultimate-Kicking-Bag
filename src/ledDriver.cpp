#include <Arduino.h>
#include "ledDriver.h"

CRGB leds[NUM_LEDS];

void ledDriver::setup(){
    LEDS.addLeds<WS2812,PIN,GRB>(leds,NUM_LEDS);
    clearLeds();
}

void ledDriver::setLeds(String side){
    clearLeds();
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

void ledDriver::sensorStatusLight(bool status){
    if(status){
        FastLED.setBrightness(50);
        for (int i = 0; i < 24; i++){
            leds[i] = CRGB::Green;
        }
        FastLED.show();
        delayMicroseconds(2000000);
        clearLeds();
        delayMicroseconds(2000000);
        FastLED.setBrightness(50);
        for (int i = 0; i < 24; i++){
            leds[i] = CRGB::Green;
        }
        FastLED.show();
        delayMicroseconds(2000000);
        clearLeds();
        leds[0] = CRGB::Green;
        leds[1] = CRGB::Green;
        leds[23] = CRGB::Green;
        FastLED.setBrightness(20);
        FastLED.show();
    } else{
        FastLED.setBrightness(50);
        for (int i = 0; i < 24; i++){
            leds[i] = CRGB::Red;
        }
        FastLED.show();
        delayMicroseconds(2000000);
        clearLeds();
        delayMicroseconds(2000000);
        FastLED.setBrightness(50);
        for (int i = 0; i < 24; i++){
            leds[i] = CRGB::Red;
        }
        FastLED.show();
        delayMicroseconds(2000000);
        clearLeds();
        leds[0] = CRGB::Red;
        leds[1] = CRGB::Red;
        leds[23] = CRGB::Red;
        FastLED.setBrightness(20);
        FastLED.show();
    }
}

void ledDriver::hitLedSignal(){
    FastLED.setBrightness(20);
    for (int i = 0; i < 24; i++){
        leds[i] = CRGB::LightBlue;
    }
    FastLED.show();
    delayMicroseconds(1000000);
    clearLeds();
}

void ledDriver::clearLeds()
{
	FastLED.clear();
	FastLED.setBrightness(0);
	FastLED.show();
}