#include <gamemodes.h>

void gamemodes::fastestKick(ledDriver &ledRing, webserver &webServer)
{
    setGlobalDataVariables(false, true, "Busy", 0);
    webServer.updateFrontEnd();

    previousStartSignalTrigger = millis();
    interval = random(2000, 6000);
    gameIsReadyToPlay = true;

    while (gameIsReadyToPlay)
    {
        if (isStartSignalTriggered(millis(), ledRing))
        {
            while (gameIsPlaying)
            {
                if ((globalData.sensorValue - globalData.calibratedMedianValue) > globalData.difficulty)
                {
                    endGame(ledRing, webServer);
                }
                vTaskDelay(20);
            }
        }
        vTaskDelay(20);
    }
}

//Experimental gamemode
void gamemodes::fastestAmountOfKicks(ledDriver &ledRing, webserver &webServer)
{
    setGlobalDataVariables(false, true, "Busy", 0);
    webServer.updateFrontEnd();

    previousStartSignalTrigger = millis();
    interval = random(2000, 5000);
    gameIsReadyToPlay = true;

    bool amountAchieved = false;
    int8_t amountToAchieve = 5;
    int8_t amountOfKicks = 0;

    int kickInterval = 300;
    unsigned long previousKick = 0;

    while (gameIsReadyToPlay)
    {
        if (isStartSignalTriggered(millis(), ledRing))
        {
            while (gameIsPlaying)
            {
                if ((globalData.sensorValue - globalData.calibratedMedianValue) > globalData.difficulty)
                {
                    if ((millis() - previousKick) >= kickInterval)
                    {
                        amountOfKicks++;
                        Serial.println("Kick!!!!!!!!!!!!!!!!!!!!");
                        previousKick = millis();
                    }
                }

                if (amountOfKicks >= amountToAchieve)
                {
                    endGame(ledRing, webServer);
                }
            }
        }
    }
}

void gamemodes::setGlobalDataVariables(bool triggerButton, bool enableSensor, String systemStatus, unsigned long reactionTime)
{
    globalData.triggerButton = triggerButton;
    globalData.enableSensor = enableSensor;
    globalData.systemStatus = systemStatus;
    globalData.reactionTime = reactionTime;
}

bool gamemodes::isStartSignalTriggered(unsigned long currentTime, ledDriver &ledRing)
{
    if ((currentTime - previousStartSignalTrigger) >= interval)
    {
        previousStartSignalTrigger = currentTime;
        gameIsReadyToPlay = false;
        gameIsPlaying = true;
        ledRing.setLeds(globalData.ledSide);
        startGameTime = millis();
        return true;
    }
    else
    {
        return false;
    }
}

void gamemodes::endGame(ledDriver &ledRing, webserver &webServer)
{
    endGameTime = millis();
    ledRing.hitLedSignal();
    gameIsPlaying = false;

    setGlobalDataVariables(false, false, "Ready", (endGameTime - startGameTime));
    webServer.updateFrontEnd();
}