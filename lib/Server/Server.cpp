#include "HttpHandle.h"
#include "Server.h"

#if defined(ESP32)
#include <AsyncElegantOTA.h>
#endif

AsyncWebServer server(3000);

Route routes[] = {
    {"/wifi", HTTP_POST, HandleWifi},
    {"/color", HTTP_POST, HandleColor},
    {"/mode", HTTP_POST, HandleMode},
    {"/time", HTTP_POST, HandleTime},
    {"/info", HTTP_POST, HandleInfo}}; // harusnya HTTP_GET tapi ketika mengunakan mthod get mendapatkan masalah
int numRoutes = sizeof(routes) / sizeof(Route);

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

    json["code"] = 404;
    json["message"] = "NOT FOUND";

    const String url = request->url();
    const int httpMethod = request->method(); // HTTP_GET: 1 | HTTP_POST: 2 | HTTP_DELETE: 4 | HTTP_PUT: 8 | HTTP_PATCH: 16
    Serial.print("New request to ");
    Serial.print(url);
    Serial.print(" with method : ");
    Serial.print(httpMethod);
    Serial.print("\n");

    for (int i = 0; i < numRoutes; i++)
    {
      if ((routes[i].path == url) && (routes[i].method == httpMethod))
      {
        routes[i].handle(request, jsonDoc, json);
        break;
      }
    }

    String jsonString;
    serializeJson(json, jsonString);
    request->send(json["code"].as<int>(), "application/json", jsonString);
  }
}

void httpServer()
{
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  // DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "content-type");

  server.onRequestBody(handleRequest);

  server.on(
      "/", HTTP_GET, [](AsyncWebServerRequest *request )
      { request->send(200, "text/plain", DEVICES_NAME); });

  server.on(
      "/ping", HTTP_GET, [](AsyncWebServerRequest *request)
      { request->send(200, "text/plain", "pong"); });

  server.on(
      "/reset", HTTP_DELETE, [](AsyncWebServerRequest *request)
      { 
        resetEEPROM();
        request->send(200, "application/json", "{}");
        delay(500);
        ESP.restart(); });

  server.on(
      "/restart", HTTP_DELETE, [](AsyncWebServerRequest *request)
      { 
        request->send(200, "application/json", "{}");
        delay(500);
        ESP.restart(); });

  server.onNotFound([](AsyncWebServerRequest *request)
                    { request->send(404, "application/json", "{\"status\": \"Not found\"}"); });

#if defined(ESP32)
  AsyncElegantOTA.begin(&server, APssid.c_str(), APpassword.c_str());
#endif
  Serial.println("Starting server...");
  server.begin();
  Serial.println("Server started");
}
