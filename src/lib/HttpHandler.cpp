#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <EEPROM.h>
#include <AsyncElegantOTA.h>
#include <FastLED.h>
#include "HttpHandler.h"
#include "EEPROMFunc.h"
#include "../Var.h"
#include "Led.h"

AsyncWebServer server(3000);
bool LED2_State = false;

// WEB SERVER FUNCTION
void httpHandler()
{
    server.on("/resetall", HTTP_POST, [](AsyncWebServerRequest *request)
              {
    defaultState();
    request->send(200, "application/json", "{}"); 
    delay(500);
    ESP.restart(); });

    server.on("/wifi/set", HTTP_POST, [](AsyncWebServerRequest *request)
              {
    bool valuesExist = (request->hasArg("ssid")) && (request->hasArg("password"));

    if (valuesExist){
      String ssid = request->arg("ssid");
      String password = request->arg("password");

      saveWifiCredentials(ssid.c_str(), password.c_str());
      request->send(200, "application/json", "{\"status\": \"OK\"}"); 
      delay(100);
      ESP.restart();
    } else {
      request->send(400, "application/json", "{\"status\": \"Bad request\"}");
    } });

    server.on("/ping", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "application/json", "{\"status\": \"OK\"}"); });

    server.on("/led2", HTTP_POST, [](AsyncWebServerRequest *request)
              {
    LED2_State = !LED2_State;
    digitalWrite(2, LED2_State);
    request->send(200, "application/json", "{\"status\": \"OK\"}"); });

    server.on("/color/change", HTTP_POST, [](AsyncWebServerRequest *request)
              {
    bool valuesExist = (request->hasArg("red")) && (request->hasArg("green")) && (request->hasArg("blue")) && (request->hasArg("brightness"));

    if (valuesExist){
      const int red = atoi(request->arg("red").c_str());
      const int green = atoi(request->arg("green").c_str());
      const int blue = atoi(request->arg("blue").c_str());
      const int brightness = atoi(request->arg("brightness").c_str());

      Serial.print("---------- set color red : ");
      Serial.println(red);
      Serial.print("---------- set color green : ");
      Serial.println(green);
      Serial.print("---------- set color blue : ");
      Serial.println(blue);
      Serial.print("---------- set brightness : ");
      Serial.println(brightness);

      EEPROM.put(RED_ADDRESS, red);
      EEPROM.put(GREEN_ADDRESS, green);
      EEPROM.put(BLUE_ADDRESS, blue);
      //  EEPROM.put(BRIGHTNESS_ADDRESS, brightness);
      EEPROM.commit();

      FastLED.setBrightness(brightness);
      FastLED.show();
      refreshColor();

      request->send(200, "application/json", "{\"status\": \"OK\"}"); 
    } else {
      request->send(400, "application/json", "{\"status\": \"Bad request\"}");
    } });

    server.on("/color/mode", HTTP_POST, [](AsyncWebServerRequest *request)
              {
                  String mode = request->arg("mode");

                  ColorMode = mode;
                  request->send(200, "application/json", "{\"status\": \"OK\"}");
              });

    server.on("/brightness", HTTP_POST, [](AsyncWebServerRequest *request)
              {
    bool valuesExist = (request->hasArg("brightness"));

    if (valuesExist){
      const int brightness = atoi(request->arg("brightness").c_str());
      FastLED.setBrightness(brightness);
      FastLED.show();
      request->send(200, "application/json", "{\"status\": \"OK\"}"); 
    } else {
      request->send(400, "application/json", "{\"status\": \"Bad request\"}");
    } });

    server.on("/timeformat", HTTP_POST, [](AsyncWebServerRequest *request)
              {
    if (request->hasArg("format")){
      int mode = atoi(request->arg("format").c_str());
      timeFormat = mode;
      EEPROM.put(TIME_FORMAT_ADDRESS, mode);  // BLUE COLOR
      EEPROM.commit();
      
      request->send(200, "application/json", "{\"status\": \"OK\"}");
    } else {
      request->send(400, "application/json", "{\"status\": \"Bad request\"}");
    } });

    server.on("/mode", HTTP_POST, [](AsyncWebServerRequest *request)
              {
    if (request->hasArg("mode")){
      int mode = atoi(request->arg("mode").c_str());
      int limit = atoi(request->arg("limit").c_str());
      displayMode = mode;

      if (mode == 2){ // counter mode
        counterCount = 0;
        counterLimit = limit;
        BlankDots();
      } else if (mode == 3) { // count down mode
        BlankDisplay(0);
        countDownCount = limit;
        BlankDots();
      }
      
      request->send(200, "application/json", "{\"status\": \"OK\"}");
    } else {
      request->send(400, "application/json", "{\"status\": \"Bad request\"}");
    } });

    server.on("/restart", HTTP_POST, [](AsyncWebServerRequest *request)
              {
    request->send(200, "application/json", "{\"status\": \"OK\"}");
    delay(500);
    ESP.restart(); });

    server.onNotFound([](AsyncWebServerRequest *request)
                      { request->send(404, "application/json", "{\"status\": \"Not found\"}"); });

    AsyncElegantOTA.begin(&server, APssid, APpassword);
    server.begin();
    Serial.println("Server started");
}
