#include "WifiFunc.h"
#include "../Var.h"
#include "Led.h"
#include "EEPROMFunc.h"

IPAddress local_ip(192, 168, 0, 1);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

void startWifiAp()
{
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(local_ip, gateway, subnet);
    WiFi.softAP(APssid, APpassword);

    Serial.println("");
    Serial.println(""); // print space

    Serial.println("Access Point started");
    Serial.print("----- ssid:");
    Serial.println(APssid);
    Serial.print("----- pwd:");
    Serial.println(APpassword);

    IPAddress localIP = WiFi.softAPIP();
    Serial.println(localIP);

    // print ip address to display
    for (int i = 0; i < 4; i++)
    {
        int octet = (int)localIP[i];
        BlankDisplay(50);
        PrintNumber(octet, "full");
        delay(1500);
    }
    displayMode = "counter";
    return;
}

void startWifiSta()
{
    int trying = 1;
    // Connect to Wi-Fi using the saved SSID and password
    WiFi.mode(WIFI_STA);
    WiFi.setHostname(APssid); // define hostname
    WiFi.begin(ssid.c_str(), password.c_str());

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.println(String(trying) + "." + "Connecting to WiFi...");
        Serial.print("----- ssid:");
        Serial.println(ssid);
        // Serial.print("----- password:");
        // Serial.println(password);
        ShowDotsRgb(0, 0, 255);

        if (digitalRead(WIFI_AP_BTN) == LOW)
        {
            BlankDots();
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

    Gateway = WiFi.gatewayIP();
    Subnet = WiFi.subnetMask();
    DNS1 = WiFi.dnsIP();
    if (isStaticIP)
    {
        WiFi.config(IP, Gateway, Subnet, DNS1);
        Serial.print("Use static ip");
    }
    else
    {
        IP = WiFi.localIP();
    }

    Serial.print("Ip address : ");
    IPAddress localIP = WiFi.localIP();
    Serial.println(localIP);

    // print ip address to display
    for (int i = 0; i < 4; i++)
    {
        int octet = (int)localIP[i];
        BlankDisplay(50);
        PrintNumber(octet, "full");
        delay(1500);
    }
    return;
}

bool isInternetConnection()
{
    WiFiClient client;

    // Connect to a test website
    if (!client.connect("www.google.com", 80))
        return false;

    // Send a HTTP GET request
    client.print("GET / HTTP/1.1\r\nHost: www.google.com\r\n\r\n");

    // Wait for response
    while (!client.available())
    {
        delay(10);
    }

    // Read response headers
    String headers = client.readStringUntil('\n');

    // Check if response is OK
    if (headers.indexOf("200 OK") == -1)
        return false;

    return true;
}
