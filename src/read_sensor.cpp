#include <Arduino.h>
#include "read_sensor.h"

bool ReadSensor::start(ICM20689 *g)
{
    gyro = g;
    return globalData.sensorWorking;
}

float ReadSensor::readAccellerometer(String axis)
{
    if (gyro == nullptr)
    {
        Serial.println("There is no sensor available");
    }

    if (gyro->readSensor() == -1)
    {
        Serial.println("HitDetection isn't able to read sensor data");
    }

    if (axis.equalsIgnoreCase("x")) {
        return gyro->getAccelX_mss();
    } else if (axis.equalsIgnoreCase("y")) {
        return gyro->getAccelY_mss();
    } else if (axis.equalsIgnoreCase("z")) {
        return gyro->getAccelZ_mss();
    } else {
        return gyro->getAccelX_mss();
    }
}

void ReadSensor::calibrateMedianValue(String axis)
{
    float tempMedianCalculation = 0;

    for (int i = 0; i < 100; i++)
    {
        tempMedianCalculation += readAccellerometer(axis);
    }

    globalData.calibratedMedianValue = abs(tempMedianCalculation / 100.00);

    Serial.print("Calibrated value: ");
    Serial.println(globalData.calibratedMedianValue);
}

void ReadSensor::printAccelValues()
{
    gyro->readSensor();
    Serial.print(gyro->getAccelX_mss());
    Serial.print(" ");
    Serial.print(gyro->getAccelY_mss());
    Serial.print(" ");
    Serial.println(gyro->getAccelZ_mss());
}

void ReadSensor::printGyroValues()
{
    gyro->readSensor();
    Serial.print(gyro->getGyroX_rads());
    Serial.print(" ");
    Serial.print(gyro->getGyroY_rads());
    Serial.print(" ");
    Serial.println(gyro->getGyroZ_rads());
}