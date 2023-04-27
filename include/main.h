#pragma once
#include "WiFi.h"
#include "SPIFFS.h"
#include "ICM20689.h"
#include "settings.h"

#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"

//Function prototypes
void gyroscope(void * parameter);
void asyncWebServer(void * parameter);
void runGame(void * parameter);