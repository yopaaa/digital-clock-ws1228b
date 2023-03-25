#include "Var.h"
#include <Arduino.h>

String CodeVersion = "v1.3.1";
String ssid = "";
String password = "";

const char *APssid = "digital_clock";
const char *APpassword = "12345678zxcvbnm";

const int SSID_ADDRESS = 400;
const int PASSWORD_ADDRESS = 432;
const int RED_ADDRESS = 101;
const int GREEN_ADDRESS = 102;
const int BLUE_ADDRESS = 103;
const int TIME_FORMAT_ADDRESS = 120;

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


