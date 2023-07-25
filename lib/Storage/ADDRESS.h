#ifndef ADDRESS_H
#define ADDRESS_H

#define RED_ADDRESS 1
#define GREEN_ADDRESS 2
#define BLUE_ADDRESS 3
#define BRIGHTNESS_ADDRESS 4

#define TIME_FORMAT_ADDRESS 8 // this address use to store time format 12 | 24
#define GMT_OFFSET_ADDRESS 10  // this address use to store gmt offset variable type is long, to store in eeprom he use 4 address

#define SEGMENT_1_MODE_ADDRESS 15 // this address use to store string segment mode 1
#define SEGMENT_2_MODE_ADDRESS 25 // this address use to store string segment mode 2

#define IP_ADDRESS_START_ADDRESS 36 // this address use to store static ip, to store ip in eeprom he use 4 address
#define IS_STATIC_IP_ADDRESS 40     // this address use to store boolean is static ip or not

#define SSID_ADDRESS 100     // this address use to store STA ssid maks length is 15
#define PASSWORD_ADDRESS 115 // this address use to store STA ssid maks length is 35

#define AP_SSID_ADDRESS 150     // this address use to store AP ssid maks length is 15
#define AP_PASSWORD_ADDRESS 165 // this address use to store AP ssid maks length is 35

#define ALARM_LIST_ADDRESS 200 // this address use to store alarm list

#define DEVICESS_ID_ADDRESS 400 // this address use to store alarm list

#endif