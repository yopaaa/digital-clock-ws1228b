#include <AsyncElegantOTA.h>
#include "HttpHandler.h"
#include "EEPROMFunc.h"
#include "../Var.h"
#include "Led.h"
#include "WifiFunc.h"

AsyncWebServer server(3000);

void stringToIP(String ipAddress, int ipArray[])
{
  int dot1 = ipAddress.indexOf(".");
  ipArray[0] = ipAddress.substring(0, dot1).toInt();

  int dot2 = ipAddress.indexOf(".", dot1 + 1);
  ipArray[1] = ipAddress.substring(dot1 + 1, dot2).toInt();

  int dot3 = ipAddress.indexOf(".", dot2 + 1);
  ipArray[2] = ipAddress.substring(dot2 + 1, dot3).toInt();

  ipArray[3] = ipAddress.substring(dot3 + 1).toInt();
}

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
        writeStaticIp(nullIP, nullIP, nullIP);
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
    else if (url == "/staticIp")
    {
      if ((jsonDoc.containsKey("isStaticIP")))
      {
        bool extractisStaticIP = jsonDoc["isStaticIP"].as<bool>();

        bool isChangeStaticIp = (jsonDoc.containsKey("ip")) && (jsonDoc.containsKey("gateway")) && (jsonDoc.containsKey("subnet"));
        if (isChangeStaticIp)
        {
          String ip = jsonDoc["ip"].as<String>();
          String gateway = jsonDoc["gateway"].as<String>();
          String subnet = jsonDoc["subnet"].as<String>();

          int octetIp[4];
          int octetGateway[4];
          int octetSubnet[4];

          stringToIP(ip, octetIp);
          stringToIP(gateway, octetGateway);
          stringToIP(subnet, octetSubnet);

          IPAddress combineIp(octetIp[0], octetIp[1], octetIp[2], octetIp[3]);
          IPAddress combineGateway(octetGateway[0], octetGateway[1], octetGateway[2], octetGateway[3]);
          IPAddress combineSubnet(octetSubnet[0], octetSubnet[1], octetSubnet[2], octetSubnet[3]);

          writeStaticIp(combineIp, combineGateway, combineSubnet);
          writeBool(IS_STATIC_IP_ADDRESS, extractisStaticIP);
          payload["ip"] = combineIp;
          payload["gateway"] = combineGateway;
          payload["subnet"] = combineSubnet;
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
  payload["IP"] = IP;
  payload["Gateway"] = Gateway;
  payload["Subnet"] = Subnet;

  String jsonString;
  serializeJson(json, jsonString);

  request->send(200, "application/json", jsonString);
}

void handleTestInternetConnection(AsyncWebServerRequest *request)
{
  DynamicJsonDocument json(1224);
  JsonObject payload = json.createNestedObject("payload");
  json["code"] = 200;
  json["message"] = "OK";
  json["method"] = request->method();
  json["url"] = request->url();
  json["host"] = request->host();

  payload["isInternetConnection"] = isInternetConnection();

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
  server.on("/isInternetConnection", HTTP_GET, handleTestInternetConnection);

  server.onNotFound([](AsyncWebServerRequest *request)
                    { request->send(404, "application/json", "{\"status\": \"Not found\"}"); });

  AsyncElegantOTA.begin(&server, APssid, APpassword);
  server.begin();
  Serial.println("Server started");
}
