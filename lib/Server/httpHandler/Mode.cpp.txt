#include "HttpHandle.h"

void HandleMode(AsyncWebServerRequest *request, DynamicJsonDocument &jsonDoc, DynamicJsonDocument &json)
{
    JsonObject payload = json.createNestedObject("payload");
    JsonObject info = json.createNestedObject("info");

    json["code"] = 200;
    json["message"] = "OK";

    if (jsonDoc.containsKey("mode"))
    {
        String mode = jsonDoc["mode"].as<String>();
        setMode(mode);
    }

    if (jsonDoc.containsKey("colorMode"))
    {
        String colormode = jsonDoc["colorMode"].as<String>();
        colorMode = colormode;
    }

    if (jsonDoc.containsKey("limit"))
    {
        int limit = jsonDoc["limit"].as<int>();

        if (Mode == "counter") // counter mode
        {
            counterCount = 0;
            counterLimit = limit;
        }
        else if (Mode == "countdown") // count down mode
        {
            BlankDisplay(0);
            countDownCount = limit;
        }
    }

    if (jsonDoc.containsKey("scors1"))
    {
        int scorsOne = jsonDoc["scors1"].as<int>();
        scors1 = scorsOne;
    }

    if (jsonDoc.containsKey("scors2"))
    {
        int scorsTwo = jsonDoc["scors2"].as<int>();
        scors2 = scorsTwo;
    }

    if (jsonDoc.containsKey("segment1"))
    {
        String segment1 = jsonDoc["segment1"].as<String>();
        writeString(SEGMENT_1_MODE_ADDRESS, segment1);
        segment1mode = segment1;
    }

    if (jsonDoc.containsKey("segment2"))
    {
        String segment2 = jsonDoc["segment2"].as<String>();
        writeString(SEGMENT_2_MODE_ADDRESS, segment2);
        segment2mode = segment2;
    }

    if (jsonDoc.containsKey("isPause"))
    {
        bool ispause = jsonDoc["isPause"].as<bool>();
        isPause = ispause;
    }

    payload["mode"] = Mode;
    payload["colorMode"] = colorMode;
    payload["segment1"] = segment1mode;
    payload["segment2"] = segment2mode;
    payload["scors1"] = scors1;
    payload["scors2"] = scors2;
    payload["isPause"] = isPause;
    payload["limit"] = counterLimit;

    info["counterCount"] = counterCount;
    info["countDownCount"] = countDownCount;
}
