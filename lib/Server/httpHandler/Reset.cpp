#include "HttpHandle.h"

void HandleReset(AsyncWebServerRequest *request, DynamicJsonDocument &jsonDoc, DynamicJsonDocument &json)
{
    JsonObject payload = json.createNestedObject("payload");
    resetEEPROM();
    request->send(200, "application/json", "{}");
    delay(500);
    ESP.restart();
}
