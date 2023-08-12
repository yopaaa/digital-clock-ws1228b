#include "WifiFunc.h"
#include "Led.h"
#include "Mode.h"
#include "mqtt.h"

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <AsyncTCP.h>
#endif

void startWifiSta()
{
    if (ssid.length() < 1)
    {
        startWifiAp();
        return;
    }

    String hostName = DEVICES_NAME + "-" + DEVICES_ID;
    WiFi.setHostname(hostName.c_str());
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str()); // Connect to Wi-Fi using the saved SSID and password
    BlankDots();
    int trying = 1;

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print("\n" + String(trying) + "." + "Connecting to WiFi...");
        Serial.print("\n----- ssid:");
        Serial.print(ssid);
        Serial.print("\n----- password:");
        Serial.print(password);
        ShowDotsRgb(255, 0, 0);

        if (trying == 100)
        {
            Serial.print("\ncant connect to network");
            ESP.restart();
        }
        trying++;
        delay(1000);
    }
    Serial.print("\nConnected to WiFi");

    Gateway = WiFi.gatewayIP();
    Subnet = WiFi.subnetMask();
    DNS1 = WiFi.dnsIP();
    if (isStaticIP)
    {
        WiFi.config(IP, Gateway, Subnet, DNS1);
        Serial.print("\nUse static ip");
    }
    else
    {
        IP = WiFi.localIP();
    }

    Serial.print("\nIp address : ");
    Serial.print(IP);

    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer.c_str());
    setupMqtt();
    return;
}
