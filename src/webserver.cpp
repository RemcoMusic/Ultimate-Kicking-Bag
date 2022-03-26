#include <Arduino.h>
#include "webserver.h"

AsyncWebSocketClient * globalClient = NULL;

String getFrontEndData(){
  DynamicJsonDocument updateData(1024);
  updateData["Gamemode"] = globalData.gamemode;
  updateData["LedDirection"] = globalData.ledSide;
  updateData["SystemStatus"] = globalData.systemStatus;

    if(globalData.difficulty == 10){
      updateData["Difficulty"] = "Adult";
    }else {
       updateData["Difficulty"] = "Kid";
    }
    float result = (globalData.reactionTime/1000.00);
    updateData["ReactionTime"] = result;

  String output;
  serializeJson(updateData, output);
  return output;
}

void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
  if(type == WS_EVT_CONNECT){
    Serial.println("Websocket client connection received");
    globalClient = client;
    globalClient->text(getFrontEndData());
  } else if(type == WS_EVT_DISCONNECT){
    Serial.println("Websocket client connection finished");
    globalClient = NULL;
  }
}

void webserver::startAsyncWebServer(){
  ws = new AsyncWebSocket("/update");
  server = new AsyncWebServer(80);

  server->serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
  
  server->on("/start", HTTP_GET, [](AsyncWebServerRequest *request){ 
    globalData.triggerButton = true;
    request->send(200);
  });

  server->on("/restart", HTTP_GET, [](AsyncWebServerRequest *request){ 
    ESP.restart();
    request->send(200);
  });

  server->on("/left", HTTP_GET, [](AsyncWebServerRequest *request){  
    globalData.ledSide = "left";
    request->send(200);
  });

  server->on("/right", HTTP_GET, [](AsyncWebServerRequest *request){  
    globalData.ledSide = "right";
    request->send(200);
  });

  server->on("/random", HTTP_GET, [](AsyncWebServerRequest *request){  
    globalData.ledSide = "random";
    request->send(200);
  });

  server->on("/fast", HTTP_GET, [](AsyncWebServerRequest *request){  
    globalData.gamemode = "fast";
    request->send(200);
  });

  server->on("/amount", HTTP_GET, [](AsyncWebServerRequest *request){  
    globalData.gamemode = "amount";
    request->send(200);
  });

  server->on("/printAccel", HTTP_GET, [](AsyncWebServerRequest *request){  
    globalData.gamemode = "printAccel";
    request->send(200);
  });

  server->on("/printGyro", HTTP_GET, [](AsyncWebServerRequest *request){  
    globalData.gamemode = "printGyro";
    request->send(200);
  });

  server->on("/adult", HTTP_GET, [](AsyncWebServerRequest *request){  
    globalData.difficulty = 10;
    request->send(200);
  });

  server->on("/kid", HTTP_GET, [](AsyncWebServerRequest *request){  
    globalData.difficulty = 3;
    request->send(200);
  });

  ws->onEvent(onWsEvent);
  server->addHandler(ws);
  server->begin();
}

void webserver::updateFrontEnd(){
  if(globalClient != NULL && globalClient->status() == WS_CONNECTED){
    globalClient->text(getFrontEndData());
  } else {
    Serial.println("No clients are connected");
  }
}