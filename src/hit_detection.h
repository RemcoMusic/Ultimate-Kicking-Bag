#include <Arduino.h>
#include "MPU9250.h"
#include "settings.h"

class HitDetection{
    private:
        MPU9250* gyro = nullptr;

    public:
        bool startSensor(MPU9250* g);
        float readSensor();
};