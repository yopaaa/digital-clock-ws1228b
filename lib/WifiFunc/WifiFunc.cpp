#include "WifiFunc.h"
#include "Var.h"
#include "Led.h"
#include "EEPROMFunc.h"

IPAddress softAPLocalIP(192, 168, 0, 1);
IPAddress softAPGateway(192, 168, 0, 1);
IPAddress softAPSubnet(255, 255, 255, 0);

void printIpAddressToDisplay(IPAddress ip)
{
    Serial.print("Ip address : ");
    Serial.println(ip);
    // print ip address to display
    for (int i = 0; i < 4; i++)
    {
        int octet = (int)ip[i];
        BlankDisplay(50);
        PrintNumber(octet, "full");
        delay(1500);
    }
    return;
}

void checkIsWiFiConnected()
{
    int trying = 1;

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.println(String(trying) + "." + "Connecting to WiFi...");
        Serial.print("----- ssid:");
        Serial.println(ssid);
        ShowDotsRgb(255, 0, 0);

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
    Serial.println("Connected to WiFi");
    return;
}

void startWifiAp()
{
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(softAPLocalIP, softAPGateway, softAPSubnet);
    WiFi.softAP(APssid, APpassword);

    Serial.println("Access Point started");
    Serial.print("----- ssid:");
    Serial.println(APssid);
    Serial.print("----- pwd:");
    Serial.println(APpassword);

    IPAddress localIP = WiFi.softAPIP();
    printIpAddressToDisplay(localIP);
    displayMode = "counter";
    return;
}

void startWifiSta()
{
    WiFi.mode(WIFI_STA);
    WiFi.setHostname(APssid);                   // define hostname
    WiFi.begin(ssid.c_str(), password.c_str()); // Connect to Wi-Fi using the saved SSID and password
    checkIsWiFiConnected();
    BlankDots();
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

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
    if (headers.indexOf("200 OK") == -1)
        return false;

    return true;
}
