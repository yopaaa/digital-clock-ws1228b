#ifndef HTTP_HANDLER
#define HTTP_HANDLER

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "Storage.h"
#include "WifiFunc.h"

#include "Buzzer.h"
#include "ADDRESS.h"
#include "Led.h"
#include "Alarm.h"
#include "Mode.h"

struct Route
{
    String path;
    int method;
    void (*handle)(AsyncWebServerRequest *, DynamicJsonDocument &, DynamicJsonDocument &);
};

extern Route routes[];
extern int numRoutes;
void HandleWifi(AsyncWebServerRequest *request, DynamicJsonDocument &jsonDoc, DynamicJsonDocument &json);
void HandleColor(AsyncWebServerRequest *request, DynamicJsonDocument &jsonDoc, DynamicJsonDocument &json);
void HandleMode(AsyncWebServerRequest *request, DynamicJsonDocument &jsonDoc, DynamicJsonDocument &json);
void HandleTime(AsyncWebServerRequest *request, DynamicJsonDocument &jsonDoc, DynamicJsonDocument &json);
void HandleInfo(AsyncWebServerRequest *request, DynamicJsonDocument &jsonDoc, DynamicJsonDocument &json);

#endif