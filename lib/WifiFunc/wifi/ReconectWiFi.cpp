#include "WifiFunc.h"
#include "Led.h"
#include "Mode.h"

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESP8266Ping.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESP32Ping.h>
#endif

void reconnectWiFiSTA()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str()); // Connect to Wi-Fi using the saved SSID and password
    String hostName = DEVICES_NAME + DEVICES_ID;
    WiFi.setHostname(hostName.c_str());
    WiFi.config(IP, Gateway, Subnet, DNS1);
    Serial.print("\nConnecting to WiFi....");
    return;
}

unsigned long previousMillis1 = 0;

void reconnectWiFi()
{
    unsigned long currentMillis = millis();

    if ((currentMillis - previousMillis1 >= (interval * 30)) && WiFi.getMode() == WIFI_STA)
    {
        previousMillis1 = currentMillis;

        if (Ping.ping(Gateway, 1))
        {
            Serial.print("\nPing success to ");
            Serial.print(Gateway);
        }
        else
        {
            reconnectWiFiSTA();
            Serial.print("\nPing failed to ");
            Serial.print(Gateway);
        }
    }
}
