#include "WifiFunc.h"
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <AsyncTCP.h>
#endif
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
