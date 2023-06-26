#include "HttpHandle.h"

void HandleTime(AsyncWebServerRequest *request, DynamicJsonDocument &jsonDoc, DynamicJsonDocument &json)
{
    JsonObject payload = json.createNestedObject("payload");
    JsonObject info = json.createNestedObject("info");

    json["code"] = 200;
    json["message"] = "OK";

    if ((jsonDoc.containsKey("timestamp")))
    {
        int timestamp = jsonDoc["timestamp"].as<int>();
        timeval tv = {timestamp, 0};
        settimeofday(&tv, nullptr);

        payload["timestamp"] = timestamp;
    }

    if ((jsonDoc.containsKey("format")))
    {
        int format = jsonDoc["format"].as<int>();
        timeFormat = format;
        write(TIME_FORMAT_ADDRESS, format);
    }

    if ((jsonDoc.containsKey("gmtOffset_sec")))
    {
        long gmtOffset = jsonDoc["gmtOffset_sec"].as<long>();
        writeLong(GMT_OFFSET_ADDRESS, gmtOffset);
    }

    if (jsonDoc.containsKey("alarms"))
    {
        JsonArray alarmsArray = jsonDoc["alarms"]; // Dapatkan array "alarms"

        for (JsonObject alarm : alarmsArray)
        {
            bool isValidSetAlarm = (jsonDoc.containsKey("index")) && (jsonDoc.containsKey("min")) && (jsonDoc.containsKey("hour")) && (jsonDoc.containsKey("days")) && (jsonDoc.containsKey("alertIndex"));
            if (isValidSetAlarm)
            {
                int index = alarm["index"].as<int>();
                int hour = alarm["hour"].as<int>();
                int min = alarm["min"].as<int>();
                int days = alarm["days"].as<int>();
                int alertIndex = alarm["alertIndex"].as<int>();

                int binaryArray[7] = {0};

                hexToBinaryArray(days, binaryArray);
                setAlarm(index, binaryArray, hour, min, alertIndex);
                writeAlarmsToEEPROM();
            }
            else
            {
                json["code"] = 400;
                json["message"] = "Bad Request";
            }
        }
    }
    
    payload["format"] = timeFormat;
    payload["gmtOffset_sec"] = gmtOffset_sec;

    time_t now;
    struct tm timeinfo;
    time(&now);
    payload["timestamp"] = now;

    info["ntpServer"] = ntpServer;
    info["daylightOffset_sec"] = daylightOffset_sec;

    JsonArray alarams = payload.createNestedArray("alarms");
    for (int i = 0; i < MAX_ALARMS; i++)
    {
        JsonObject alarmJson = alarams.createNestedObject();
        alarmJson["hour"] = alarms[i].hour;
        alarmJson["minute"] = alarms[i].min;
        alarmJson["alertIndex"] = alarms[i].alertIndex;

        JsonArray daysArray = alarmJson.createNestedArray("days");
        for (int j = 0; j < 7; j++)
        {
            daysArray.add(alarms[i].days[j]);
        }
    }
}
