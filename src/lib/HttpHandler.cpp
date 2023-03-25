#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <EEPROM.h>
#include <AsyncElegantOTA.h>
#include <FastLED.h>
#include <ArduinoJson.h>
#include "HttpHandler.h"
#include "EEPROMFunc.h"
#include "../Var.h"
#include "Led.h"

AsyncWebServer server(3000);
bool LED2_State = false;

void handleRequest(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
  // Check if this is the first data packet
  if (index == 0)
  {
    // Create a DynamicJsonDocument object with the same size as the incoming JSON data
    DynamicJsonDocument jsonDoc(len);
    // Parse the incoming JSON data into the DynamicJsonDocument object
    DeserializationError error = deserializeJson(jsonDoc, data);
    // Check if the JSON parsing was successful
    if (error)
    {
      request->send(400, "text/plain", "Bad Request");
      return;
    }

    DynamicJsonDocument json(1024);
    // json["name"] = jsonDoc["name"].as<String>();
    // json["age"] = jsonDoc["age"].as<int>();
    json["code"] = 200;
    json["message"] = "OK";
    json["version"] = request->version();
    json["method"] = request->method();
    json["url"] = request->url();
    json["host"] = request->host();
    json["contentType"] = request->contentType();
    json["contentLength"] = request->contentLength();
    json["multipart"] = request->multipart();

    const String url = request->url();
    if (url == "/led2")
    {
      json["pesan"] = jsonDoc["pesan"].as<String>();
      LED2_State = !LED2_State;
      digitalWrite(2, LED2_State);
    }
    else
    {
      json["code"] = 404;
      json["message"] = "NOT FOUND";
    }

    String jsonString;
    serializeJson(json, jsonString);
    request->send(json["code"].as<int>(), "application/json", jsonString);
  }
}

void handlePing(AsyncWebServerRequest *request)
{
  DynamicJsonDocument json(1024);
  json["message"] = "OK";
  json["CodeVersion"] = CodeVersion;
  json["CycleCount"] = ESP.getCycleCount();
  // json["ChipModel"] = ESP.getChipModel();
  json["SketchSize"] = ESP.getSketchSize();
  json["version"] = request->version();
  json["method"] = request->method();
  json["url"] = request->url();
  json["host"] = request->host();
  json["contentType"] = request->contentType();
  json["contentLength"] = request->contentLength();
  json["multipart"] = request->multipart();

  String jsonString;
  serializeJson(json, jsonString);

  request->send(200, "application/json", jsonString);
}

 void httpHandler()
{
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "content-type");

  server.onRequestBody(handleRequest);

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

  server.on("/ping", HTTP_GET, handlePing);

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
                  request->send(200, "application/json", "{\"status\": \"OK\"}"); });

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
      String mode = request->arg("mode");
      int limit = atoi(request->arg("limit").c_str());
      displayMode = mode;

      if (mode == "counter"){ // counter mode
        counterCount = 0;
        counterLimit = limit;
        BlankDots();
      } else if (mode == "countdown") { // count down mode
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
