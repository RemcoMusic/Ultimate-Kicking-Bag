#include <gamemodes.h>

void gamemodes::fastestKick(ledDriver &ledRing, webserver &webServer){
    globalData.triggerButton = false;
    globalData.enableSensor = true;
    globalData.systemStatus = "Busy";
    webServer.updateFrontEnd();

    //Variables for start time
    long previousTriggerMillis = 0;
    unsigned long currentTriggerMillis = 0;
    long interval = 0;

    //Variables for measering hit time
    unsigned long currentStartTime = 0;
    unsigned long currentEndTime= 0;
    long elapsedTime = 0;

    currentTriggerMillis = millis();
    previousTriggerMillis = millis();
    interval = random(2000,6000);

    bool kicked = true;
    bool game = true;

    while (game){
        currentTriggerMillis = millis();
        
        if (currentTriggerMillis - previousTriggerMillis >= interval){
            previousTriggerMillis = currentTriggerMillis;
            game=false;
            kicked = false;
            ledRing.setLeds(globalData.ledSide);
            currentStartTime = millis();

            while(!kicked){
                if((globalData.sensorValue - globalData.calibratedMedianValue) > globalData.difficulty){
                    currentEndTime = millis();
                    elapsedTime = currentEndTime - currentStartTime;

                    globalData.systemStatus = "Ready";
                    globalData.reactionTime = elapsedTime;
                    ledRing.hitLedSignal();
                    kicked = true;
                    globalData.enableSensor = false;
                    webServer.updateFrontEnd();
                }
                vTaskDelay(10);
            }
        }
        vTaskDelay(20);
    }
}

void gamemodes::fastestAmountOfKicks(ledDriver &ledRing, webserver &webServer){
    globalData.triggerButton = false;
    globalData.enableSensor = true;
    // webServer.setSystemStatus(false);

    //Variables for start time
    long previousTriggerMillis = 0;
    unsigned long currentTriggerMillis = 0;
    long interval = 0;

    //Variables for measering hit time
    unsigned long currentStartTime = 0;
    unsigned long currentEndTime= 0;
    long elapsedTime = 0;

    currentTriggerMillis = millis();
    previousTriggerMillis = millis();
    interval = random(2000,5000);

    bool game = true;
    bool amountAchieved = false;
    int8_t amountToAchieve = 5;
    int8_t amountOfKicks = 0;

    int kickInterval = 300;
    unsigned long previousKick = 0;

    while(game){
        currentTriggerMillis = millis();
        
        if (currentTriggerMillis - previousTriggerMillis >= interval){
            previousTriggerMillis = currentTriggerMillis;
            ledRing.setLeds(globalData.ledSide);
            game = false;
            currentStartTime = millis();

            while(!amountAchieved){
                delay(10);
                if((globalData.sensorValue - globalData.calibratedMedianValue) > globalData.difficulty){
                    if((millis() - previousKick) >= kickInterval){
                        amountOfKicks++;
                        Serial.println("Kick!!!!!!!!!!!!!!!!!!!!");
                        previousKick = millis();
                    }
                }

                if(amountOfKicks >= amountToAchieve){
                    currentEndTime = millis();
                    elapsedTime = currentEndTime - currentStartTime;

                    // webServer.setTime(elapsedTime);
                    // webServer.setSystemStatus(true);
                    ledRing.hitLedSignal();
                    amountAchieved = true;
                    globalData.enableSensor = false;
                }
            }
        }
    }
}