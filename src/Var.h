#ifndef VAR
#define VAR

#include <Arduino.h>

#define LED_TYPE WS2812B
#define NUMBER_SEGMENT 4
#define LEDS_PER_SEGMENT 7
#define NUM_LEDS 28
#define NUM_DOTS 2
#define LED_PIN 0     // Seven segment pin
#define DOTS_PIN 3    // Dots pin
#define WIFI_AP_BTN 2 // Start wifi mode AP pin button

extern String CodeVersion;
extern String ssid;
extern String password;

extern const char *APssid;
extern const char *APpassword;

extern const int SSID_ADDRESS;
extern const int PASSWORD_ADDRESS;
extern const int RED_ADDRESS;
extern const int GREEN_ADDRESS;
extern const int BLUE_ADDRESS;
extern const int TIME_FORMAT_ADDRESS;

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

#endif