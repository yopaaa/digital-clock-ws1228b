#include "WifiFunc.h"
#include "Buzzer.h"
#include "Led.h"
#include "Mode.h"

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <AsyncTCP.h>
#endif

IPAddress softAPLocalIP(192, 168, 15, 1);
IPAddress softAPGateway(192, 168, 15, 1);
IPAddress softAPSubnet(255, 255, 255, 0);

void startWifiAp()
{
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(softAPLocalIP, softAPGateway, softAPSubnet);
    WiFi.softAP(APssid.c_str(), APpassword.c_str());

    myBuzzer.beep(50);
    Serial.println("Access Point started");
    Serial.print("----- ssid:");
    Serial.println(APssid);
    Serial.print("----- pwd:");
    Serial.println(APpassword);

    IPAddress localIP = WiFi.softAPIP();
    printIpAddressToDisplay(localIP);
    setMode("CLI");
    ShowDotsRgb(0, 0, 255);
    return;
}


