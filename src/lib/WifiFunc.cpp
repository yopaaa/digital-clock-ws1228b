#include "WifiFunc.h"
#include "../Var.h"
#include "Led.h"
#include <Arduino.h>

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <AsyncTCP.h>
#endif

#include <ESPAsyncWebServer.h>
// WIFI FUNCTION

IPAddress local_ip(192, 168, 0, 1);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

void startWifiAp()
{
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(local_ip, gateway, subnet);
    WiFi.softAP(APssid, APpassword, 1, 0, 1);

    Serial.println("");
    Serial.println(""); // print space

    Serial.println("Access Point started");
    Serial.print("----- ssid:");
    Serial.println(APssid);
    Serial.print("----- pwd:");
    Serial.println(APpassword);
    Serial.println(WiFi.softAPIP());
    displayMode = "counter";
}

void startWifiSta()
{
    int trying = 1;

    // Connect to Wi-Fi using the saved SSID and password
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.println(String(trying) + "." + "Connecting to WiFi...");
        Serial.print("----- ssid:");
        Serial.println(ssid);
        ShowDotsRgb(0, 0, 255);

        if (digitalRead(WIFI_AP_BTN) == LOW)
        {
            startWifiAp();
            return;
        };
        if (trying == 100)
        {
            Serial.println("cant connect to network");
            ESP.restart();
            return;
        }
        trying++;
        delay(1000);
    }

    BlankDots();
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    Serial.println("Connected to WiFi");
    Serial.print("Ip address : ");

    IPAddress localIP = WiFi.localIP();
    Serial.println(localIP);

    // print ip address to display
    for (int i = 0; i < 4; i++)
    {
        int octet = (int)localIP[i];
        BlankDisplay(50);
        if (octet > 99) // example 123
        {
            int firstDigit = octet / 100;
            int secondDigit = (octet % 100) / 10;
            int thirdDigit = octet % 10;

            SetNumber(1, 13); // print _
            SetNumber(2, firstDigit);
            SetNumber(3, secondDigit);
            SetNumber(4, thirdDigit);
        }
        else if (octet > 9) // example 67
        {
            int firstDigit = octet / 10;
            int secondDigit = octet % 10;

            SetNumber(2, 13); // print _
            SetNumber(3, firstDigit);
            SetNumber(4, secondDigit);
        }
        else
        {                     // example 8
            SetNumber(2, 13); // print _
            SetNumber(4, octet);
        }
        delay(1000);
    }
    delay(2000);
    return;
}
