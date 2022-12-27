#include <Arduino.h>
#include "MPU9250.h"
#include "settings.h"

class ReadSensor{
    private:
        MPU9250* gyro = nullptr;

    public:
        bool start(MPU9250* g);
        float readAccellerometer(String axis);
        void calibrateMedianValue(String axis);
        void printAccelValues();
        void printGyroValues();
};