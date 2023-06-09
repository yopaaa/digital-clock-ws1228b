#include "HttpHandler.h"
#include "EEPROMFunc.h"
#include "Var.h"
#include "Led.h"
#include "WifiFunc.h"

#if defined(ESP32)
#include <AsyncElegantOTA.h>
#endif

AsyncWebServer server(3000);

void handleRequest(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
  if (index == 0)
  {
    DynamicJsonDocument jsonDoc(len);
    DeserializationError error = deserializeJson(jsonDoc, data);
    if (error)
    {
      request->send(500, "text/plain", "500 Internal Server Error");
      return;
    }

    DynamicJsonDocument json(1024);
    JsonObject payload = json.createNestedObject("payload");

    json["code"] = 200;
    json["message"] = "OK";
    json["method"] = request->method();
    json["url"] = request->url();

    const String url = request->url();
    Serial.print("New request to ");
    Serial.print(url);
    Serial.print("\n");

    if (url == "/set/wifi/sta")
    {
      if ((jsonDoc.containsKey("ssid")) && (jsonDoc.containsKey("password")))
      {
        String ssid = jsonDoc["ssid"].as<String>();
        String password = jsonDoc["password"].as<String>();
        IPAddress nullIP(0, 0, 0, 0);

        writeString(SSID_ADDRESS, ssid);
        writeString(PASSWORD_ADDRESS, password);
        writeStaticIp(nullIP);

        payload["ssid"] = ssid;
        payload["password"] = password;
      }
      else
      {
        json["code"] = 400;
        json["message"] = "Bad Request";
      }
    }
    else if (url == "/set/wifi/ap")
    {
      if ((jsonDoc.containsKey("ssid")) && (jsonDoc.containsKey("password")))
      {
        String ssid = jsonDoc["ssid"].as<String>();
        String password = jsonDoc["password"].as<String>();

        writeString(AP_SSID_ADDRESS, ssid);
        writeString(AP_PASSWORD_ADDRESS, password);

        payload["ssid"] = ssid;
        payload["password"] = password;
      }
      else
      {
        json["code"] = 400;
        json["message"] = "Bad Request";
      }
    }
    else if (url == "/set/mode")
    {
      if ((jsonDoc.containsKey("mode")) && (jsonDoc.containsKey("limit")))
      {
        String mode = jsonDoc["mode"].as<String>();
        int limit = jsonDoc["limit"].as<int>();
        setMode(mode);

        if (mode == "counter")
        { // counter mode
          counterCount = 0;
          counterLimit = limit;
        }
        else if (mode == "countdown")
        { // count down mode
          BlankDisplay(0);
          countDownCount = limit;
        }
        BlankDots();
        payload["mode"] = mode;
        payload["limit"] = limit;
      }
      else
      {
        json["code"] = 400;
        json["message"] = "Bad Request";
      }
    }
    else if (url == "/set/scors")
    {
      if ((jsonDoc.containsKey("scors1")) && (jsonDoc.containsKey("scors2")))
      {
        int scorsOne = jsonDoc["scors1"].as<int>();
        int scorsTwo = jsonDoc["scors2"].as<int>();

        scors1 = scorsOne;
        scors2 = scorsTwo;

        payload["scors1"] = scors1;
        payload["scors2"] = scors2;
      }
      else
      {
        json["code"] = 400;
        json["message"] = "Bad Request";
      }
    }
    else if (url == "/set/time/format")
    {
      if ((jsonDoc.containsKey("format")))
      {
        int format = jsonDoc["format"].as<int>();

        timeFormat = format;
        EEPROM.put(TIME_FORMAT_ADDRESS, format); // BLUE COLOR
        EEPROM.commit();
        payload["format"] = format;
      }
      else
      {
        json["code"] = 400;
        json["message"] = "Bad Request";
      }
    }
    else if (url == "/set/time")
    {
      if ((jsonDoc.containsKey("timestamp")))
      {
        int timestamp = jsonDoc["timestamp"].as<int>();
        timeval tv = {timestamp, 0};
        settimeofday(&tv, nullptr);

        payload["timestamp"] = timestamp;
      }
      else
      {
        json["code"] = 400;
        json["message"] = "Bad Request";
      }
    }
    else if (url == "/set/gmtOffset_sec")
    {
      if ((jsonDoc.containsKey("gmtOffset_sec")))
      {
        long gmtOffset = jsonDoc["gmtOffset_sec"].as<long>();
        writeLong(GMT_OFFSET_ADDRESS, gmtOffset);

        payload["gmtOffset"] = gmtOffset;
      }
      else
      {
        json["code"] = 400;
        json["message"] = "Bad Request";
      }
    }
    else if (url == "/set/segment")
    {
      if ((jsonDoc.containsKey("segment1")) && (jsonDoc.containsKey("segment2")))
      {
        String segment1 = jsonDoc["segment1"].as<String>();
        String segment2 = jsonDoc["segment2"].as<String>();
        writeString(SEGMENT_1_MODE_ADDRESS, segment1);
        writeString(SEGMENT_2_MODE_ADDRESS, segment2);
        segment1mode = segment1;
        segment2mode = segment2;
        payload["segment1mode"] = segment1;
        payload["segment2mode"] = segment2;
      }
      else
      {
        json["code"] = 400;
        json["message"] = "Bad Request";
      }
    }
    else if (url == "/set/brightness")
    {
      if ((jsonDoc.containsKey("brightness")))
      {
        int brightness = jsonDoc["brightness"].as<int>();
        FastLED.setBrightness(brightness);
        FastLED.show();
        payload["brightness"] = brightness;
      }
      else
      {
        json["code"] = 400;
        json["message"] = "Bad Request";
      }
    }
    else if (url == "/set/color")
    {
      if ((jsonDoc.containsKey("red")) && (jsonDoc.containsKey("green")) && (jsonDoc.containsKey("blue")))
      {
        int red = jsonDoc["red"].as<int>();
        int green = jsonDoc["green"].as<int>();
        int blue = jsonDoc["blue"].as<int>();

        if ((jsonDoc.containsKey("brightness")))
        {
          int brightness = jsonDoc["brightness"].as<int>();
          FastLED.setBrightness(brightness);
          FastLED.show();
          payload["brightness"] = brightness;
        }
        EEPROM.put(RED_ADDRESS, red);
        EEPROM.put(GREEN_ADDRESS, green);
        EEPROM.put(BLUE_ADDRESS, blue);
        EEPROM.commit();
        readColor();

        payload["red"] = red;
        payload["green"] = green;
        payload["blue"] = blue;
      }
      else
      {
        json["code"] = 400;
        json["message"] = "Bad Request";
      }
    }
    else if (url == "/set/color/mode")
    {
      if ((jsonDoc.containsKey("mode")))
      {
        String mode = jsonDoc["mode"].as<String>();

        ColorMode = mode;
        payload["mode"] = mode;
      }
      else
      {
        json["code"] = 400;
        json["message"] = "Bad Request";
      }
    }
    else if (url == "/set/staticIp")
    {
      if ((jsonDoc.containsKey("isStaticIP")))
      {
        bool extractisStaticIP = jsonDoc["isStaticIP"].as<bool>();

        bool isChangeStaticIp = (jsonDoc.containsKey("ip"));
        if (isChangeStaticIp)
        {
          String ip = jsonDoc["ip"].as<String>();

          int octetIp[4];

          int dot1 = ip.indexOf(".");
          octetIp[0] = ip.substring(0, dot1).toInt();

          int dot2 = ip.indexOf(".", dot1 + 1);
          octetIp[1] = ip.substring(dot1 + 1, dot2).toInt();

          int dot3 = ip.indexOf(".", dot2 + 1);
          octetIp[2] = ip.substring(dot2 + 1, dot3).toInt();

          octetIp[3] = ip.substring(dot3 + 1).toInt();

          IPAddress combineIp(octetIp[0], octetIp[1], octetIp[2], octetIp[3]);

          writeStaticIp(combineIp);
          writeBool(IS_STATIC_IP_ADDRESS, extractisStaticIP);
          payload["ip"] = combineIp;
        }

        payload["isStaticIP"] = extractisStaticIP;
      }
      else
      {
        json["code"] = 400;
        json["message"] = "Bad Request";
      }
    }
    else // NOT FOUND Handle
    {
      json["code"] = 404;
      json["message"] = "NOT FOUND";
    }

    String jsonString;
    serializeJson(json, jsonString);
    request->send(json["code"].as<int>(), "application/json", jsonString);
  }
}

void handleVariable(AsyncWebServerRequest *request)
{
  DynamicJsonDocument json(1224);
  JsonObject Mutable = json.createNestedObject("mutable");
  json["code"] = 200;
  json["message"] = "OK";
  json["method"] = request->method();
  json["url"] = request->url();
  json["args"] = request->args();
  json["params"] = request->params();
  json["CODE_VERSION"] = CODE_VERSION;
  json["uptime"] = millis() / 1000;
  json["isInternetConnection"] = isInternetConnection();

  Mutable["ssid"] = ssid;
  Mutable["password"] = password;
  Mutable["APssid"] = APssid;
  Mutable["APpassword"] = APpassword;
  Mutable["ntpServer"] = ntpServer;
  Mutable["gmtOffset_sec"] = gmtOffset_sec;
  Mutable["daylightOffset_sec"] = daylightOffset_sec;

  Mutable["RED"] = RED;
  Mutable["GREEN"] = GREEN;
  Mutable["BLUE"] = BLUE;
  Mutable["BRIGHTNESS"] = BRIGHTNESS;
  Mutable["ColorMode"] = ColorMode;

  Mutable["Mode"] = Mode;
  Mutable["RefreshDelay"] = RefreshDelay;
  Mutable["counterCount"] = counterCount;
  Mutable["counterLimit"] = counterLimit;
  Mutable["countDownCount"] = countDownCount;
  Mutable["timeFormat"] = timeFormat;
  Mutable["segment1mode"] = segment1mode;
  Mutable["segment2mode"] = segment2mode;

  Mutable["isStaticIP"] = isStaticIP;
  Mutable["IP"] = IP;
  Mutable["Gateway"] = Gateway;
  Mutable["Subnet"] = Subnet;
  Mutable["DNS1"] = DNS1;

  String jsonString;
  serializeJson(json, jsonString);

  request->send(200, "application/json", jsonString);
}

void httpHandler()
{
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  // DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "content-type");

  server.onRequestBody(handleRequest);

  server.on("/resetall", HTTP_POST, [](AsyncWebServerRequest *request)
            {
    defaultState();
    request->send(200, "application/json", "{}"); 
    delay(500);
    ESP.restart(); });

  server.on("/restart", HTTP_POST, [](AsyncWebServerRequest *request)
            {
    request->send(200, "application/json", "{}"); 
    delay(500);
    ESP.restart(); });

  server.on("/variable", HTTP_GET, handleVariable);
  server.on(
      "/ping", HTTP_GET, [](AsyncWebServerRequest *request)
      { request->send(200, "application/json", "{\"status\": \"OK\"}"); });

  server.onNotFound([](AsyncWebServerRequest *request)
                    { request->send(404, "application/json", "{\"status\": \"Not found\"}"); });

#if defined(ESP32)
  AsyncElegantOTA.begin(&server, APssid.c_str(), APpassword.c_str());
#endif
  Serial.println("Starting server...");
  server.begin();
  Serial.println("Server started");
}
