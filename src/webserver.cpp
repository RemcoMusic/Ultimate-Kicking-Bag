#include <Arduino.h>
#include "webserver.h"

AsyncWebSocketClient * globalClient = NULL;

String getFrontEndData(){
  DynamicJsonDocument updateData(1024);
  updateData["Gamemode"] = globalData.gamemode;
  updateData["LedDirection"] = globalData.ledSide;
  updateData["Difficulty"];
  updateData["SystemStatus"] = globalData.systemStatus;

    if(globalData.difficulty == 10){
      updateData["Difficulty"] = "Adult";
    }else {
       updateData["Difficulty"] = "Kid";
    }

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
  ws = new AsyncWebSocket("/ws");
  server = new AsyncWebServer(80);

  server->serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
  
  server->on("/start", HTTP_GET, [](AsyncWebServerRequest *request){ 
    globalData.triggerButton = true;
    Serial.println("Start button is clicked");
  });

  // server->on("/restart", HTTP_GET, [](AsyncWebServerRequest *request){ 
  //       ESP.restart();
  //   request->send(SPIFFS, "/index.html", String(), false, processor);
  // });

  // server->on("/left", HTTP_GET, [](AsyncWebServerRequest *request){  
  //     globalData.ledSide = "left";
  //   request->send(SPIFFS, "/index.html", String(), false, processor);
  // });

  // server->on("/right", HTTP_GET, [](AsyncWebServerRequest *request){  
  //     globalData.ledSide = "right";
  //   request->send(SPIFFS, "/index.html", String(), false, processor);
  // });

  // server->on("/random", HTTP_GET, [](AsyncWebServerRequest *request){  
  //     globalData.ledSide = "random";
  //   request->send(SPIFFS, "/index.html", String(), false, processor);
  // });

  // server->on("/fast", HTTP_GET, [](AsyncWebServerRequest *request){  
  //     globalData.gamemode = "fast";
  //   request->send(SPIFFS, "/index.html", String(), false, processor);
  // });

  // server->on("/amount", HTTP_GET, [](AsyncWebServerRequest *request){  
  //     globalData.gamemode = "amount";
  //   request->send(SPIFFS, "/index.html", String(), false, processor);
  // });

  // server->on("/printAccel", HTTP_GET, [](AsyncWebServerRequest *request){  
  //     globalData.gamemode = "printAccel";
  //   request->send(SPIFFS, "/index.html", String(), false, processor);
  // });

  // server->on("/printGyro", HTTP_GET, [](AsyncWebServerRequest *request){  
  //     globalData.gamemode = "printGyro";
  //   request->send(SPIFFS, "/index.html", String(), false, processor);
  // });

  // server->on("/adult", HTTP_GET, [](AsyncWebServerRequest *request){  
  //     globalData.difficulty = 10;
  //   request->send(SPIFFS, "/index.html", String(), false, processor);
  // });

  // server->on("/kid", HTTP_GET, [](AsyncWebServerRequest *request){  
  //     globalData.difficulty = 3;
  //   request->send(SPIFFS, "/index.html", String(), false, processor);
  // });

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

void webserver::setTime(long reactionTime){
  if(globalClient != NULL && globalClient->status() == WS_CONNECTED){
      float timeInSeconds = (reactionTime/1000.00);
      String convTimeToString = String(timeInSeconds);
      globalClient->text(convTimeToString);
   }
}

void webserver::setSystemStatus(bool status){
  if(globalClient != NULL && globalClient->status() == WS_CONNECTED){
      String message = "Failed";
      if(status){
        message = "Ready";
        globalData.systemStatus = message;
      } else{
        message = "Busy";
        globalData.systemStatus = message;
      }
      globalClient->text(message);
   }
}