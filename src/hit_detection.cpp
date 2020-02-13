#include <Arduino.h>
#include "hit_detection.h"

//MPU 9250 - Setup
MPU9250 IMU(Wire,0x68);
int status;

void HitDetection::startSensor(){
    status = IMU.begin();
}

float HitDetection::readSensor(){
    IMU.readSensor();
    return IMU.getAccelX_mss();
}