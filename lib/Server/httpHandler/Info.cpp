#include "HttpHandle.h"

void HandleInfo(AsyncWebServerRequest *request, DynamicJsonDocument &jsonDoc, DynamicJsonDocument &json)
{
    JsonArray endpoint = json.createNestedArray("endpoint");
    String host = request->host();

    json["code"] = 200;
    json["message"] = "OK";
    json["method"] = request->method();
    json["url"] = request->url();
    json["args"] = request->args();
    json["params"] = request->params();
    json["CODE_VERSION"] = CODE_VERSION;
    json["uptime"] = millis() / 1000;
    json["isInternetConnection"] = isInternetConnection();
    json["flashChipSize"] = ESP.getFlashChipSize();
    json["SketchSize"] = ESP.getSketchSize();

    for (int i = 0; i < numRoutes; i++)
    {
        endpoint.add(host + routes[i].path);
    }

    String jsonString;
    serializeJson(json, jsonString);
    request->send(200, "application/json", jsonString);
}
