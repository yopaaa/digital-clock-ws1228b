#ifndef WIFI_FUNC
#define WIFI_FUNC

#include <Arduino.h>
#include "ADDRESS.h"

extern String DEVICES_NAME;
extern String DEVICES_ID;

#include <IPAddress.h>

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
void reconnectWiFi();

#endif