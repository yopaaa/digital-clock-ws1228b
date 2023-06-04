#ifndef VAR
#define VAR

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

#define LED_TYPE WS2812B
#define NUMBER_SEGMENT 4
#define LEDS_PER_SEGMENT 7
#define NUM_LEDS 28
#define NUM_DOTS 2

#define LED_PIN 23    // Seven segment pin
#define WIFI_AP_BTN 4 // Start wifi mode AP pin button

#define CODE_VERSION "v3.5.25" // tahun 202'3', bulan .., tanggal ..

#define SSID_ADDRESS 400
#define PASSWORD_ADDRESS 415

#define RED_ADDRESS 101
#define GREEN_ADDRESS 102
#define BLUE_ADDRESS 103
#define COLOR_MODE_ADDRESS 105

#define TIME_FORMAT_ADDRESS 120
#define IP_ADDRESS_START_ADDRESS 200
#define IS_STATIC_IP_ADDRESS 205

extern String ssid;
extern String password;

extern const char *APssid;
extern const char *APpassword;

extern const char *ntpServer;
extern const long gmtOffset_sec; // UTC+7
extern const int daylightOffset_sec;

extern byte RED;
extern byte GREEN;
extern byte BLUE;
extern byte BRIGHTNESS;

extern String ColorMode;

extern String displayMode;
extern int RefreshDelay;
extern int counterCount;
extern int counterLimit;
extern int countDownCount;
extern int timeFormat;
extern String segment1mode;
extern String segment2mode;

extern bool isStaticIP;
extern IPAddress IP;
extern IPAddress Gateway;
extern IPAddress Subnet;
extern IPAddress DNS1;

#endif