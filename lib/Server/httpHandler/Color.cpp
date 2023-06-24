#include "HttpHandle.h"

void HandleColor(AsyncWebServerRequest *request, DynamicJsonDocument &jsonDoc, DynamicJsonDocument &json)
{
    JsonObject payload = json.createNestedObject("payload");
    JsonObject info = json.createNestedObject("info");

    json["code"] = 200;
    json["message"] = "OK";

    if (jsonDoc.containsKey("red"))
    {
        int red = jsonDoc["red"].as<int>();
        write(RED_ADDRESS, red);
        readColor();
    }

    if (jsonDoc.containsKey("green"))
    {
        int green = jsonDoc["green"].as<int>();
        write(GREEN_ADDRESS, green);
        readColor();
    }

    if (jsonDoc.containsKey("blue"))
    {
        int blue = jsonDoc["blue"].as<int>();
        write(BLUE_ADDRESS, blue);
        readColor();
    }

    if (jsonDoc.containsKey("brightness"))
    {
        int brightness = jsonDoc["brightness"].as<int>();
        SetBrightness(brightness);
    }

    payload["red"] = RED;
    payload["green"] = GREEN;
    payload["blue"] = BLUE;
    payload["brightness"] = BRIGHTNESS;
}
