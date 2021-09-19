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

void HitDetection::calibrateMedianValue(){
    float tempMedianCalculation = 0;

    for (int i = 0; i < 100; i++){
        tempMedianCalculation += readSensor();
    }

    globalData.calibratedMedianValue = abs(tempMedianCalculation/100.00);
  
    Serial.print("Calibrated value: ");
    Serial.println(globalData.calibratedMedianValue);
}

void HitDetection::printAccelValues(){
    gyro->readSensor();
    Serial.print(gyro->getAccelX_mss());
    Serial.print(" ");
    Serial.print(gyro->getAccelY_mss());
    Serial.print(" ");
    Serial.println(gyro->getAccelZ_mss());
}

void HitDetection::printGyroValues(){
    gyro->readSensor();
    Serial.print(gyro->getGyroX_rads());
    Serial.print(" ");
    Serial.print(gyro->getGyroY_rads());
    Serial.print(" ");
    Serial.println(gyro->getGyroZ_rads());
}