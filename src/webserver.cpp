#include <Arduino.h>
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "webserver.h"

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
AsyncWebSocketClient * globalClient = NULL;

bool a1 = false;
bool triggerButton = false;
String timeMessage;
String errorMessage;

// Replaces placeholder with LED state value
String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    if(a1){
      timeMessage = "ON";
    }
    else{
      timeMessage = "OFF";
    }
    Serial.print(timeMessage);
    return timeMessage;
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
    // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  server.on("/start", HTTP_GET, [](AsyncWebServerRequest *request){ 
        triggerButton = true;
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request){  
      a1 = false;
      triggerButton = false;
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  ws.onEvent(onWsEvent);
  server.addHandler(&ws);
  server.begin();
}

void webserver::setTime(long reactionTime){
  if(globalClient != NULL && globalClient->status() == WS_CONNECTED){
      float timeInSeconds = (reactionTime/1000.00);
      String convTimeToString = String(timeInSeconds);
      globalClient->text(convTimeToString);
   }
}

void webserver::resetTriggerButton(){
  triggerButton = false;
}

bool webserver::readTriggerButton(){
  return triggerButton;
}
