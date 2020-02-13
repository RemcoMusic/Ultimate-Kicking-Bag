/*#include <Arduino.h>
#include <Wire.h>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "MPU9250.h"

// Replace with your network credentials
const char* ssid = "Taekwondo-Papendrecht";
const char* password = "Test1234";

// Set LED GPIO
const int ledPin = 2;
// Stores LED state
String ledState;
// Enable timer
bool enableTimer;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

//MPU 9250 - Setup
MPU9250 IMU(Wire,0x68);
int status;
String gyroState;

//Timers
unsigned long StartMillis;
unsigned long currentMillis;
long randomTime;
bool resetTime = true;
bool kickReset;

// Replaces placeholder with LED state value
String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    if(digitalRead(ledPin)){
      ledState = "ON";
    }
    else{
      ledState = "OFF";
    }
    Serial.print(ledState);
    return ledState;
  }
  if(var == "STATUS"){
    if(status < 0){
      gyroState = "Offline";
    }
    else{
      gyroState = "Online";
    }
    Serial.print(gyroState);
    return gyroState;
  }
  return String();
}

void initilizeGyro(){
  status = IMU.begin();
  if (status < 0) {
    Serial.println("Het zit nog steeds niet goed!");
    Serial.print("Status: ");
    Serial.println(status);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  status = IMU.begin();
  if (status < 0) {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
  }

  Serial.print("Setting AP (Access Point)…");
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  server.on("/start", HTTP_GET, [](AsyncWebServerRequest *request){
    enableTimer = true;    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request){
    kickReset = false;
    enableTimer = false;
    digitalWrite(ledPin, LOW);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  server.on("/resetGyro", HTTP_GET, [](AsyncWebServerRequest *request){
    initilizeGyro();    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  server.begin();

  StartMillis = millis();
}

void loop(){

  if(enableTimer)
  {
    if(resetTime) {
      randomTime = (60*1000);
      resetTime = false;
    }


    currentMillis = millis();

    if(currentMillis - StartMillis >= randomTime){
      digitalWrite(ledPin, HIGH);
      resetTime = true;
      kickReset = true;
      while(kickReset) {
        IMU.readSensor();
        Serial.println(IMU.getAccelX_mss(),6);
      }
    } 
  }
}*/