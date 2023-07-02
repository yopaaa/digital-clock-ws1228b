#include "WifiFunc.h"

String ssid = "";
String password = "";

String APssid = "";
String APpassword = "";

bool isStaticIP = false;
IPAddress IP(0, 0, 0, 0);
IPAddress Gateway(0, 0, 0, 0);
IPAddress Subnet(0, 0, 0, 0);
IPAddress DNS1(0, 0, 0, 0);
