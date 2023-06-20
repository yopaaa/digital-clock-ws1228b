#ifndef HTTP_HANDLER
#define HTTP_HANDLER

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
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

void HandleSetColor(AsyncWebServerRequest *request, DynamicJsonDocument &jsonDoc, DynamicJsonDocument &json);
void HandleSetMode(AsyncWebServerRequest *request, DynamicJsonDocument &jsonDoc, DynamicJsonDocument &json);

#endif