#include <Arduino.h>
#include "Wire.h"
#include "MPU9250.h"

class HitDetection {
    public:
        void startSensor();
        float readSensor();
};