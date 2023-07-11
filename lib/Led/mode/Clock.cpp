#include "Led.h"
#include "Alarm.h"
#include "Buzzer.h"
#include "Mode.h"

struct tm timeinfo;
unsigned long previousMillis = 0;

void printLocalTime()
{
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval)
    {
        previousMillis = currentMillis;

        if (!getLocalTime(&timeinfo))
        {
            unsigned long currentMillis = millis() / 1000;
            if (currentMillis >= 100)
                ESP.restart();

            Serial.println("Failed to obtain time");
            configTime(gmtOffset_sec, daylightOffset_sec, ntpServer.c_str());
            ErrorDisplay(0, 2);
            return;
        }

        checkAlarm(timeinfo.tm_wday, timeinfo.tm_hour, timeinfo.tm_min);
        BlinkDots();

        // Segmtnt 1 left
        if (segment1mode == "hour")
        {
            if ((timeFormat == 12) && (timeinfo.tm_hour > 12))
            {
                PrintNumber(timeinfo.tm_hour - 12, "left");
            }
            else
            {

                PrintNumber(timeinfo.tm_hour, "left");
            }
        }
        else if (segment1mode == "isdst")
        {
            PrintNumber(timeinfo.tm_isdst, "left");
        }
        else if (segment1mode == "mday")
        {
            PrintNumber(timeinfo.tm_mday, "left");
        }
        else if (segment1mode == "min")
        {
            PrintNumber(timeinfo.tm_min, "left");
        }
        else if (segment1mode == "mon")
        {
            PrintNumber(timeinfo.tm_mon, "left");
        }
        else if (segment1mode == "sec")
        {
            PrintNumber(timeinfo.tm_sec, "left");
        }
        else if (segment1mode == "wday")
        {
            PrintNumber(timeinfo.tm_wday, "left");
        }
        else if (segment1mode == "yday")
        {
            PrintNumber(timeinfo.tm_yday, "left");
        }
        else if (segment1mode == "year")
        {
            PrintNumber(timeinfo.tm_year, "left");
        }

        // Segmtnt 2 right
        if (segment2mode == "hour")
        {
            if ((timeFormat == 12) && (timeinfo.tm_hour > 12))
            {
                PrintNumber(timeinfo.tm_hour - 12, "right");
            }
            else
            {
                PrintNumber(timeinfo.tm_hour, "right");
            }
        }
        else if (segment2mode == "isdst")
        {
            PrintNumber(timeinfo.tm_isdst, "right");
        }
        else if (segment2mode == "mday")
        {
            PrintNumber(timeinfo.tm_mday, "right");
        }
        else if (segment2mode == "min")
        {
            PrintNumber(timeinfo.tm_min, "right");
        }
        else if (segment2mode == "mon")
        {
            PrintNumber(timeinfo.tm_mon, "right");
        }
        else if (segment2mode == "sec")
        {
            PrintNumber(timeinfo.tm_sec, "right");
        }
        else if (segment2mode == "wday")
        {
            PrintNumber(timeinfo.tm_wday, "right");
        }
        else if (segment2mode == "yday")
        {
            PrintNumber(timeinfo.tm_yday, "right");
        }
        else if (segment2mode == "year")
        {
            PrintNumber(timeinfo.tm_year, "right");
        }
    }
}
