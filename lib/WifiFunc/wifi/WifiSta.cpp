#include "WifiFunc.h"
#include "Led.h"
#include "Mode.h"

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

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str()); // Connect to Wi-Fi using the saved SSID and password
    WiFi.setHostname(DEVICES_NAME);
    BlankDots();
    int trying = 1;

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.println(String(trying) + "." + "Connecting to WiFi...");
        Serial.print("----- ssid:");
        Serial.println(ssid);
        Serial.print("----- password:");
        Serial.println(password);
        ShowDotsRgb(255, 0, 0);

        if (trying == 100)
        {
            Serial.println("cant connect to network");
            ESP.restart();
        }
        trying++;
        delay(1000);
    }
    Serial.println("Connected to WiFi");
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer.c_str());

    Gateway = WiFi.gatewayIP();
    Subnet = WiFi.subnetMask();
    DNS1 = WiFi.dnsIP();
    if (isStaticIP)
    {
        WiFi.config(IP, Gateway, Subnet, DNS1);
        Serial.println("Use static ip");
    }
    else
    {
        IP = WiFi.localIP();
    }

    IPAddress localIP = WiFi.localIP();
    printIpAddressToDisplay(localIP);
    return;
}
