#ifndef VAR
#define VAR

#include <Arduino.h>

#define LED_TYPE WS2812B
#define NUMBER_SEGMENT 4
#define LEDS_PER_SEGMENT 7
#define NUM_LEDS 28
#define NUM_DOTS 2

#define LED_PIN 23    // Seven segment pin
#define WIFI_AP_BTN 4 // Start wifi mode AP pin button

#define CODE_VERSION "v3.6.6" // tahun 202'3', bulan .., tanggal ..

#define SSID_ADDRESS 400
#define PASSWORD_ADDRESS 415

#define RED_ADDRESS 101
#define GREEN_ADDRESS 102
#define BLUE_ADDRESS 103
#define COLOR_MODE_ADDRESS 105

#define TIME_FORMAT_ADDRESS 120
#define IP_ADDRESS_START_ADDRESS 200
#define IS_STATIC_IP_ADDRESS 205

#define GMT_OFFSET_ADDRESS 130
#define SEGMENT_1_MODE_ADDRESS 140
#define SEGMENT_2_MODE_ADDRESS 145

extern String ssid;
extern String password;

extern char *APssid;
extern char *APpassword;

extern char *ntpServer;
extern long gmtOffset_sec; // UTC+7
extern int daylightOffset_sec;

extern byte RED;
extern byte GREEN;
extern byte BLUE;
extern byte BRIGHTNESS;

extern String ColorMode;

extern String Mode;
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

extern void (*mainLoop)();

#endif