#include "Led.h"
#include "Var.h"
#include <ArduinoJson.h>

void printLocalTime()
{
    if (ColorMode == "random")
    {
        RED = random(1, 255);
        GREEN = random(1, 255);
        BLUE = random(1, 255);
    }
    BlinkDots();
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        unsigned long currentMillis = millis() / 1000;
        if (currentMillis >= 100)
            ESP.restart();

        Serial.println("Failed to obtain time");
        ErrorDisplay(0, 2);
        return;
    }
    // Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
    // int second = timeinfo.tm_sec;
    DynamicJsonDocument timeinfoJson(64);
    timeinfoJson["hour"] = timeinfo.tm_hour;
    timeinfoJson["isdst"] = timeinfo.tm_isdst;
    timeinfoJson["mday"] = timeinfo.tm_mday;
    timeinfoJson["min"] = timeinfo.tm_min;
    timeinfoJson["mon"] = timeinfo.tm_mon;
    timeinfoJson["sec"] = timeinfo.tm_sec;
    timeinfoJson["wday"] = timeinfo.tm_wday;
    timeinfoJson["yday"] = timeinfo.tm_yday;
    timeinfoJson["year"] = timeinfo.tm_year;

    if ((timeFormat == 12) && (timeinfo.tm_hour > 12))
        timeinfoJson["hour"] = timeinfo.tm_hour - 12;

    PrintNumber(timeinfoJson[segment1mode].as<int>(), "left");
    PrintNumber(timeinfoJson[segment2mode].as<int>(), "right");
    // SetNumber(3, second / 10);
    // SetNumber(4, second % 10);
}

void printCounter()
{
    if (counterCount <= counterLimit)
    {
        PrintNumber(counterCount, "full");
        counterCount++;
        return;
    }
    return;
}

void printCountDown()
{
    if (countDownCount >= 0)
    {
        PrintNumber(countDownCount, "full");
        countDownCount--;
        return;
    }
    return;
}
