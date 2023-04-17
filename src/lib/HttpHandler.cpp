#include <AsyncElegantOTA.h>
#include "HttpHandler.h"
#include "EEPROMFunc.h"
#include "../Var.h"
#include "Led.h"

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
    json["host"] = request->host();

    const String url = request->url();

    if (url == "/wifi/set")
    {
      if ((jsonDoc.containsKey("ssid")) && (jsonDoc.containsKey("password")))
      {
        String ssid = jsonDoc["ssid"].as<String>();
        String password = jsonDoc["password"].as<String>();
        IPAddress nullIP(0, 0, 0, 0);

        saveWifiCredentials(ssid, password);
        writeStaticIp(nullIP);
        payload["ssid"] = ssid;
      }
      else
      {
        json["code"] = 400;
        json["message"] = "Bad Request";
      }
    }
    else if (url == "/restart")
    {
      String jsonString;
      serializeJson(json, jsonString);
      request->send(json["code"].as<int>(), "application/json", jsonString);
      delay(500);
      ESP.restart();
      return;
    }
    else if (url == "/mode")
    {
      if ((jsonDoc.containsKey("mode")) && (jsonDoc.containsKey("limit")))
      {
        String mode = jsonDoc["mode"].as<String>();
        int limit = jsonDoc["limit"].as<int>();
        displayMode = mode;

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
    else if (url == "/timeformat")
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
    else if (url == "/brightness")
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
    else if (url == "/color/change")
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
    else if (url == "/color/mode")
    {
      if ((jsonDoc.containsKey("format")))
      {
        String format = jsonDoc["format"].as<String>();

        ColorMode = format;
        payload["format"] = format;
      }
      else
      {
        json["code"] = 400;
        json["message"] = "Bad Request";
      }
    }
    else if (url == "/staticIp")
    {
      if ((jsonDoc.containsKey("isStaticIP")))
      {
        bool extractisStaticIP = jsonDoc["isStaticIP"].as<bool>();
        byte ip1 = jsonDoc["ip1"].as<byte>() || 0;
        byte ip2 = jsonDoc["ip2"].as<byte>() || 0;
        byte ip3 = jsonDoc["ip3"].as<byte>() || 0;
        byte ip4 = jsonDoc["ip4"].as<byte>() || 0;
        IPAddress combineIp(ip1, ip2, ip3, ip4);

        writeBool(IS_STATIC_IP_ADDRESS, extractisStaticIP);
        writeStaticIp(combineIp);

        payload["ip"] = combineIp;
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

void handlePing(AsyncWebServerRequest *request)
{
  DynamicJsonDocument json(1024);
  json["message"] = "OK";
  json["CodeVersion"] = CodeVersion;
  json["CycleCount"] = ESP.getCycleCount();
  // json["ChipModel"] = ESP.getChipModel();
  json["SketchSize"] = ESP.getSketchSize();
  json["version"] = request->version();
  json["method"] = request->method();
  json["url"] = request->url();
  json["host"] = request->host();
  json["contentType"] = request->contentType();
  json["contentLength"] = request->contentLength();
  json["multipart"] = request->multipart();

  String jsonString;
  serializeJson(json, jsonString);

  request->send(200, "application/json", jsonString);
}

void handleVariable(AsyncWebServerRequest *request)
{
  DynamicJsonDocument json(1224);
  JsonObject payload = json.createNestedObject("payload");
  json["code"] = 200;
  json["message"] = "OK";
  json["method"] = request->method();
  json["url"] = request->url();
  json["host"] = request->host();

  payload["CodeVersion"] = CodeVersion;
  payload["ssid"] = ssid;
  payload["APssid"] = APssid;
  payload["APpassword"] = APpassword;

  payload["RED"] = RED;
  payload["GREEN"] = GREEN;
  payload["BLUE"] = BLUE;
  payload["BRIGHTNESS"] = BRIGHTNESS;

  payload["timeFormat"] = timeFormat;
  payload["displayMode"] = displayMode;
  payload["ColorMode"] = ColorMode;

  payload["isStaticIP"] = isStaticIP;
  payload["staticIP"] = staticIP;

  String jsonString;
  serializeJson(json, jsonString);

  request->send(200, "application/json", jsonString);
}

void httpHandler()
{
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "content-type");

  server.onRequestBody(handleRequest);

  server.on("/resetall", HTTP_POST, [](AsyncWebServerRequest *request)
            {
    defaultState();
    request->send(200, "application/json", "{}"); 
    delay(500);
    ESP.restart(); });

  server.on("/ping", HTTP_GET, handlePing);
  server.on("/variable", HTTP_GET, handleVariable);

  server.onNotFound([](AsyncWebServerRequest *request)
                    { request->send(404, "application/json", "{\"status\": \"Not found\"}"); });

  AsyncElegantOTA.begin(&server, APssid, APpassword);
  server.begin();
  Serial.println("Server started");
}
