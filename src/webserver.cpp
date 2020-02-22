#include <Arduino.h>
#include "webserver.h"

AsyncWebSocketClient * globalClient = NULL;

bool triggerButton = false;
String systemStatus;
String frontGameMode;
String frontDifficulty;

// Replaces placeholder with LED state value
String processor(const String& var){
  Serial.println(var);
  if(var == "STATUS"){
    if(globalData.triggerButton){
      systemStatus = "Busy";
    }
    else{
      systemStatus = "Ready";
    }
    return systemStatus;
  }
  if(var == "GAMEMODE"){
    frontGameMode = globalData.gamemode;
    return frontGameMode;
  }
  if(var == "DIFFICULTY"){
    if(globalData.difficulty == 10){
      frontDifficulty = "Adult";
    }else {
      frontDifficulty = "Kid";
    }
    return frontDifficulty;
  }
  return String();
}

void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){

  if(type == WS_EVT_CONNECT){

    Serial.println("Websocket client connection received");
    globalClient = client;

  } else if(type == WS_EVT_DISCONNECT){

    Serial.println("Websocket client connection finished");
    globalClient = NULL;
  }
}

void webserver::startAsyncWebServer(){
  ws = new AsyncWebSocket("/ws");
  server = new AsyncWebServer(80);

    // Route for root / web page
  server->on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Route to load style.css file
  server->on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  server->on("/start", HTTP_GET, [](AsyncWebServerRequest *request){ 
        globalData.triggerButton = true;
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  server->on("/left", HTTP_GET, [](AsyncWebServerRequest *request){  
      globalData.gamemode = "left";
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  server->on("/random", HTTP_GET, [](AsyncWebServerRequest *request){  
      globalData.gamemode = "random";
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  server->on("/right", HTTP_GET, [](AsyncWebServerRequest *request){  
      globalData.gamemode = "right";
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  server->on("/adult", HTTP_GET, [](AsyncWebServerRequest *request){  
      globalData.difficulty = 10;
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  server->on("/kid", HTTP_GET, [](AsyncWebServerRequest *request){  
      globalData.difficulty = 3;
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  ws->onEvent(onWsEvent);
  server->addHandler(ws);
  server->begin();
}

void webserver::setTime(long reactionTime){
  if(globalClient != NULL && globalClient->status() == WS_CONNECTED){
      float timeInSeconds = (reactionTime/1000.00);
      String convTimeToString = String(timeInSeconds);
      globalClient->text(convTimeToString);
   }
}