#include "Var.h"

String ssid = "";
String password = "";

const char *APssid = "digital_clock";
const char *APpassword = "12345678zxcvbnm";

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 7 * 3600; // UTC+7
const int daylightOffset_sec = 0;

byte RED = 255;
byte GREEN = 0;
byte BLUE = 0;
byte BRIGHTNESS = 100;

String ColorMode = "";

String displayMode = "clock";
int RefreshDelay = 1000;
int counterCount = 0;
int counterLimit = 120;
int countDownCount = 100;
int timeFormat = 24;

String segment1mode = "hour";
String segment2mode = "min";

bool isStaticIP = false;
IPAddress IP(0, 0, 0, 0);
IPAddress Gateway(0, 0, 0, 0);
IPAddress Subnet(0, 0, 0, 0);
IPAddress DNS1(0, 0, 0, 0);
