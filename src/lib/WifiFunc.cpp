#include "WifiFunc.h"
#include "../Var.h"
#include "Led.h"
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
// WIFI FUNCTION

IPAddress local_ip(192, 168, 0, 1);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

void startWifi(int wifiMode)
{
    if (wifiMode == 1)
    { // wifi access point mode if mode = 1
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
        displayMode = 2;
    }
    else
    { // connect to access point mode if mode = 2
        int trying = 1;
        Serial.println("");
        Serial.println(""); // print space

        // Connect to Wi-Fi using the saved SSID and password
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid.c_str(), password.c_str());
        while (WiFi.status() != WL_CONNECTED)
        {
            Serial.println(String(trying) + "." + "Connecting to WiFi...");
            Serial.print("----- ssid:");
            Serial.println(ssid);
            // Serial.print("----- pwd:");
            // Serial.println(password);
            digitalWrite(2, HIGH); // turn on led
            int buttonState = digitalRead(startWifiApBtn);
            if (buttonState == HIGH)
            {
                startWifi(1);
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

        digitalWrite(2, LOW); // turn off led
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
}
