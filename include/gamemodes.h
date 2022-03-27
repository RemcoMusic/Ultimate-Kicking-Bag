#include "ledDriver.h"
#include "webserver.h"

class gamemodes
{
public:
    void fastestKick(ledDriver &ledRing, webserver &webServer);
    void fastestAmountOfKicks(ledDriver &ledRing, webserver &webServer);

private:
    unsigned long currentStartSignalTrigger = 0;
    unsigned long previousStartSignalTrigger = 0;
    unsigned long startGameTime = 0;
    unsigned long endGameTime = 0;
    unsigned long interval = 0;
    bool gameIsReadyToPlay = false;
    bool gameIsPlaying = false;

    void setGlobalDataVariables(bool triggerButton, bool enableSensor, String systemStatus, unsigned long reactionTime);
    bool isStartSignalTriggered(unsigned long currentTime, ledDriver &ledRing);
    void endGame(ledDriver &ledRing, webserver &webServer);
};