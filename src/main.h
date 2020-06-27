#pragma once
#include "WiFi.h"
#include "SPIFFS.h"
#include "MPU9250.h"
#include "settings.h"

#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"

//Function prototypes
void gyroscope(void * parameter);
void asyncWebServer(void * parameter);