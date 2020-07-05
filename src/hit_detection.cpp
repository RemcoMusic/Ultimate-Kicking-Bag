#include <Arduino.h>
#include "hit_detection.h"

bool HitDetection::startSensor(MPU9250* g){    
    gyro = g;
    return globalData.sensorWorking;
}

float HitDetection::readSensor(){
    if(gyro == nullptr){
        Serial.println("There is no sensor available");
    }
       
    if(gyro->readSensor() == -1){
        Serial.println("HitDetection isn't able to read sensor data");
    }
  return gyro->getAccelX_mss();
}