#include "PubSubClient.h"
#include "WiFi.h"
#include "WifiFunc.h"

#include "Storage.h"
#include "Buzzer.h"
#include "ADDRESS.h"
#include "Led.h"
#include "Alarm.h"
#include "Mode.h"
#include <ArduinoJson.h>

const char *mqtt_server = "public.mqtthq.com";
const int mqtt_port = 1883;
WiFiClient espClient;
PubSubClient client(espClient);

void handleColor(byte *payload, unsigned int length)
{
    char payloadBuffer[length + 1];
    memcpy(payloadBuffer, payload, length);
    payloadBuffer[length] = '\0';

    DynamicJsonDocument jsonDoc(64);
    DeserializationError error = deserializeJson(jsonDoc, payloadBuffer);

    if (error)
    {
        Serial.print("\ndeserializeJson() failed: ");
        Serial.print(error.c_str());
        return;
    }

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
        writeByte(BRIGHTNESS_ADDRESS, brightness);
        SetBrightness(brightness);
    }

    DynamicJsonDocument jsonPayload(64);
    jsonPayload["red"] = RED;
    jsonPayload["green"] = GREEN;
    jsonPayload["blue"] = BLUE;
    jsonPayload["brightness"] = BRIGHTNESS;
    String jsonString;
    serializeJson(jsonPayload, jsonString);

    String path = DEVICES_NAME + "-" + DEVICES_ID + "-info-color";
    client.publish(path.c_str(), jsonString.c_str());
}

void handleTime(byte *payload, unsigned int length)
{
    char payloadBuffer[length + 1];
    memcpy(payloadBuffer, payload, length);
    payloadBuffer[length] = '\0';

    DynamicJsonDocument jsonDoc(512);
    DeserializationError error = deserializeJson(jsonDoc, payloadBuffer);

    if (error)
    {
        Serial.print("\ndeserializeJson() failed: ");
        Serial.print(error.c_str());
        return;
    }

    if ((jsonDoc.containsKey("timestamp")))
    {
        int timestamp = jsonDoc["timestamp"].as<int>();
        timeval tv = {timestamp, 0};
        settimeofday(&tv, nullptr);
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
        JsonObject alarmsArray = jsonDoc["alarms"];

        bool isValidSetAlarm = (alarmsArray.containsKey("index")) && (alarmsArray.containsKey("min")) && (alarmsArray.containsKey("hour")) && (alarmsArray.containsKey("days")) && (alarmsArray.containsKey("alertIndex"));
        if (isValidSetAlarm)
        {
            int index = alarmsArray["index"].as<int>();
            int hour = alarmsArray["hour"].as<int>();
            int min = alarmsArray["min"].as<int>();
            int days = alarmsArray["days"].as<int>();
            int alertIndex = alarmsArray["alertIndex"].as<int>();

            int binaryArray[7] = {0};

            hexToBinaryArray(days, binaryArray);
            setAlarm(index, binaryArray, hour, min, alertIndex);
            writeAlarmsToEEPROM();
        }
    }

    DynamicJsonDocument jsonPayload(512);
    jsonPayload["format"] = timeFormat;
    jsonPayload["gmtOffset_sec"] = gmtOffset_sec;

    time_t now;
    time(&now);
    jsonPayload["timestamp"] = now;

    jsonPayload["ntpServer"] = ntpServer;
    jsonPayload["daylightOffset_sec"] = daylightOffset_sec;

    DynamicJsonDocument alarmsPayload(512);
    JsonArray alarams = alarmsPayload.createNestedArray("alarms");

    for (int i = 0; i < MAX_ALARMS; i++)
    {
        JsonObject alarmJson = alarams.createNestedObject();
        alarmJson["hour"] = alarms[i].hour;
        alarmJson["min"] = alarms[i].min;
        alarmJson["alertIndex"] = alarms[i].alertIndex;

        JsonArray daysArray = alarmJson.createNestedArray("days");
        for (int j = 0; j < 7; j++)
        {
            daysArray.add(alarms[i].days[j]);
        }
    }

    String jsonString;
    String path = DEVICES_NAME + "-" + DEVICES_ID + "-info-time";
    serializeJson(jsonPayload, jsonString);
    client.publish(path.c_str(), jsonString.c_str());

    String alaramsString;
    String pathAlarms = DEVICES_NAME + "-" + DEVICES_ID + "-info-alarms";
    serializeJson(alarmsPayload, alaramsString);
    client.publish(pathAlarms.c_str(), alaramsString.c_str());
}

void handleMode(byte *payload, unsigned int length)
{
    char payloadBuffer[length + 1];
    memcpy(payloadBuffer, payload, length);
    payloadBuffer[length] = '\0';

    DynamicJsonDocument jsonDoc(300);
    DeserializationError error = deserializeJson(jsonDoc, payloadBuffer);

    if (error)
    {
        Serial.print("\ndeserializeJson() failed: ");
        Serial.print(error.c_str());
        return;
    }

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

    DynamicJsonDocument jsonPayload(300);
    jsonPayload["mode"] = Mode;
    jsonPayload["colorMode"] = colorMode;
    jsonPayload["segment1"] = segment1mode;
    jsonPayload["segment2"] = segment2mode;
    jsonPayload["scors1"] = scors1;
    jsonPayload["scors2"] = scors2;
    jsonPayload["isPause"] = isPause;
    jsonPayload["limit"] = counterLimit;

    jsonPayload["counterCount"] = counterCount;
    jsonPayload["countDownCount"] = countDownCount;

    String jsonString;
    serializeJson(jsonPayload, jsonString);

    String path = DEVICES_NAME + "-" + DEVICES_ID + "-info-mode";
    client.publish(path.c_str(), jsonString.c_str());
}

struct MqttTopicHandler
{
    String path;
    void (*handle)(byte *payload, unsigned int length);
};

MqttTopicHandler topicHandlers[] = {
    {"-color", handleColor},
    {"-time", handleTime},
    {"-mode", handleMode}};

const int numTopicHandlers = sizeof(topicHandlers) / sizeof(topicHandlers[0]);

void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("\nMessage arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }

    for (int i = 0; i < numTopicHandlers; i++)
    {
        String path = DEVICES_NAME + "-" + DEVICES_ID + topicHandlers[i].path;

        if (String(topic) == path)
        {
            topicHandlers[i].handle(payload, length);
            break;
        }
    }
}

void reconnect()
{
    if (!client.connected())
    {
        Serial.print("\nAttempting MQTT connection...");
        if (client.connect("ESP32Client"))
        {
            Serial.print("connected");
            for (int i = 0; i < numTopicHandlers; i++)
            {
                String path = DEVICES_NAME + "-" + DEVICES_ID + topicHandlers[i].path;
                client.subscribe(path.c_str());
                // Serial.print("\nMQTT Client subscribe to: ");
                // Serial.print(path);
            }
        }
        else
        {
            Serial.print("\nfailed, rc=");
            Serial.print(client.state());
            Serial.print(" try again in 5 seconds");
        }
    }
}

void setupMqtt()
{
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
}

void mqttLoop()
{
    if (WiFi.getMode() == WIFI_STA)
    {
        if (!client.connected())
        {
            reconnect();
        }
        client.loop();
    }
}