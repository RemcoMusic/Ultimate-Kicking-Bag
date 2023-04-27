#include <Arduino.h>
#include "ICM20689.h"
#include "settings.h"

class ReadSensor{
    private:
        ICM20689* gyro = nullptr;

    public:
        bool start(ICM20689* g);
        float readAccellerometer(String axis);
        void calibrateMedianValue(String axis);
        void printAccelValues();
        void printGyroValues();
};