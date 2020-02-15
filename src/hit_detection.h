#include <Arduino.h>
#include "MPU9250.h"

class HitDetection{
    private:
        MPU9250* gyro = nullptr;

    public:
        void startSensor(MPU9250* g);
        float readSensor();
};