#ifndef VAR
#define VAR

#include <Arduino.h>

#define CODE_VERSION "v3.6.6" // tahun 202'3', bulan .., tanggal ..
#define LED_TYPE WS2812B
#define NUMBER_SEGMENT 4
#define LEDS_PER_SEGMENT 7
#define NUM_LEDS 28
#define NUM_DOTS 2

#if defined(ESP8266)
#define LED_PIN 0
#define WIFI_AP_BTN 4
#define BUZZER_PIN 2
#elif defined(ESP32)
#define LED_PIN 23
#define WIFI_AP_BTN 4
#define BUZZER_PIN 22
#endif


#define SSID_ADDRESS 400
#define PASSWORD_ADDRESS 415
extern String ssid;
extern String password;

#define AP_SSID_ADDRESS 450
#define AP_PASSWORD_ADDRESS 465
extern String APssid;
extern String APpassword;

#define GMT_OFFSET_ADDRESS 130
extern String ntpServer;
extern long gmtOffset_sec; // UTC+7
extern int daylightOffset_sec;

#define RED_ADDRESS 101
#define GREEN_ADDRESS 102
#define BLUE_ADDRESS 103
extern byte RED;
extern byte GREEN;
extern byte BLUE;
extern byte BRIGHTNESS;

#define COLOR_MODE_ADDRESS 105
extern String ColorMode;

extern String Mode;
extern int RefreshDelay;
extern int counterCount;
extern int counterLimit;
extern int countDownCount;

extern int scors1;
extern int scors2;

#define TIME_FORMAT_ADDRESS 120
extern int timeFormat;

#define SEGMENT_1_MODE_ADDRESS 140
#define SEGMENT_2_MODE_ADDRESS 145
extern String segment1mode;
extern String segment2mode;

#define IP_ADDRESS_START_ADDRESS 200
#define IS_STATIC_IP_ADDRESS 205
extern bool isStaticIP;
extern IPAddress IP;
extern IPAddress Gateway; // auto asign on connect wifi network
extern IPAddress Subnet;  // auto asign on connect wifi network
extern IPAddress DNS1;    // auto asign on connect wifi network

extern void (*mainLoop)();

#endif