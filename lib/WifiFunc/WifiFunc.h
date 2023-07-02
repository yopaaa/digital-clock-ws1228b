#ifndef WIFI_FUNC
#define WIFI_FUNC

#include <Arduino.h>
#include "ADDRESS.h"

#define DEVICES_NAME "Digital_clock"


#include <IPAddress.h>
// #if defined(ESP8266)
// #include <IPAddress.h>
// #endif

extern String ssid;
extern String password;

extern String APssid;
extern String APpassword;

extern bool isStaticIP;
extern IPAddress IP;
extern IPAddress Gateway; // auto asign on connect wifi network
extern IPAddress Subnet;  // auto asign on connect wifi network
extern IPAddress DNS1;    // auto asign on connect wifi network

void startWifiAp();
void startWifiSta();
bool isInternetConnection();
void printIpAddressToDisplay(IPAddress ip);

#endif