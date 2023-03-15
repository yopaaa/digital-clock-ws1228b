#include "Arduino.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <EEPROM.h>
#include <time.h>
#include <FastLED.h>
#include <Wire.h>
#include <FS.h>
// #include <AsyncElegantOTA.h>
#include "Var.h"
#include "./lib/HttpHandler.h"
#include "./lib/Led.h"
#include "./lib/EEPROMFunc.h"

IPAddress local_ip(192, 168, 0, 1);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

// SETUP FUNCTION
void setup()
{
  Serial.begin(9600);
  EEPROM.begin(512);
  Wire.begin();
  initPin();
  FastLED.addLeds<LED_TYPE, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.addLeds<LED_TYPE, DOTS_PIN, GRB>(Dots, NUM_DOTS);
  FastLED.setBrightness(BRIGHTNESS); // SET BRIGHTNESS FROM EEPROM VALUE

  if (EEPROM.read(1) == 0) defaultState(); // TO SETUP FIRST RUNING EEPROM
  if (EEPROM.read(TIME_FORMAT_ADDRESS) == 0){
    timeFormat = 24;
  }else{
    timeFormat = EEPROM.read(TIME_FORMAT_ADDRESS);
  }

  // Read the saved SSID and password from EEPROM
  for (int i = SSID_ADDRESS; i < SSID_ADDRESS + 32; i++)
  {
    ssid += char(EEPROM.read(i));
  }
  for (int i = PASSWORD_ADDRESS; i < PASSWORD_ADDRESS + 64; i++)
  {
    password += char(EEPROM.read(i));
  }  

  TestStartUp();
  startWifi(0); // STA WIFI
  httpHandler();
}

void initPin(){
  pinMode(2, OUTPUT);
  pinMode(startWifiApBtn, INPUT);
  return;
}



// WIFI FUNCTION

void startWifi(int wifiMode){
  if (wifiMode == 1)
  { // wifi access point mode if mode = 1
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(local_ip, gateway, subnet);
    WiFi.softAP(APssid, APpassword, 1, 0, 1);

    Serial.println("");
    Serial.println(""); // print space

    Serial.println("Access Point started");
    Serial.print("----- ssid:");
    Serial.println(APssid);
    Serial.print("----- pwd:");
    Serial.println(APpassword);
    Serial.println(WiFi.softAPIP());
    displayMode = 2;
  } else { // connect to access point mode if mode = 2
    int trying = 1;
    Serial.println("");
    Serial.println(""); // print space

    // Connect to Wi-Fi using the saved SSID and password
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    while (WiFi.status() != WL_CONNECTED)
    {
      Serial.println(String(trying) + "." + "Connecting to WiFi...");
      Serial.print("----- ssid:");
      Serial.println(ssid);
      // Serial.print("----- pwd:");
      // Serial.println(password);
      digitalWrite(2, HIGH); // turn on led 
      int buttonState = digitalRead(startWifiApBtn);
      if (buttonState == HIGH) {
        startWifi(1);
        return;
      };
      if (trying == 100){
        Serial.println("cant connect to network");
        ESP.restart(); 
        return;
      }
      trying++;
      delay(1000);
    }

    digitalWrite(2, LOW); // turn off led
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    Serial.println("Connected to WiFi");
    Serial.print("Ip address : ");

    IPAddress localIP = WiFi.localIP();
    Serial.println(localIP); 

    // print ip address to display
    for (int i = 0; i < 4; i++)
    {
      int octet = (int)localIP[i];
      BlankDisplay(50);
      if (octet > 99) // example 123
      {
        int firstDigit = octet / 100;
        int secondDigit = (octet % 100) / 10;
        int thirdDigit = octet % 10;

        SetNumber(1, 13); // print _
        SetNumber(2, firstDigit);
        SetNumber(3, secondDigit);
        SetNumber(4, thirdDigit);
      } else if (octet > 9) // example 67
      {
        int firstDigit = octet / 10;
        int secondDigit = octet % 10;

        SetNumber(2, 13); // print _
        SetNumber(3, firstDigit);
        SetNumber(4, secondDigit);
      } else { // example 8
        SetNumber(2, 13); // print _
        SetNumber(4, octet);
      }
      delay(1000);
    }
    delay(2000);
    return;
  }
}

// LOOP FUNCTION
void loop()
{
  int buttonState = digitalRead(startWifiApBtn);
  if (buttonState == HIGH) startWifi(1);

  if (displayMode == 1) {
    printLocalTime();
    delay(RefreshDelay);
  } else if (displayMode == 2) {
    printCounter();
    delay(1000);
  } else if (displayMode == 3) {
    printCountDown();
    delay(1000);
  }
}



void printLocalTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    ErrorDisplay(0, 2);
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  int second = timeinfo.tm_sec;
  int minute = timeinfo.tm_min;
  int hour = timeinfo.tm_hour;
  if ((timeFormat == 12) && (hour >12))
  {
    hour = hour - 12;
  }

  if (ColorMode == "random")
  {
    RED = random(1,255);
    GREEN = random(1,255);
    BLUE = random(1,255);
  }
  

  SetNumber(1, hour / 10);
  SetNumber(2, hour % 10);
  SetNumber(3, minute / 10);
  SetNumber(4, minute % 10);
  // SetNumber(1, minute / 10);
  // SetNumber(2, minute % 10);
  // SetNumber(3, second / 10);
  // SetNumber(4, second % 10);
  BlinkDots();
}

void printCounter(){
  if (counterCount <= counterLimit) {
    if (counterCount < 10) { // 0-9
      BlankDisplay(0);
      SetNumber(4, counterCount);
    } else if ((counterCount > 9) && (counterCount < 100)) { // 9-99
      int firstDigit = counterCount / 10;
      int secondDigit = counterCount % 10;

      SetNumber(3, firstDigit);
      SetNumber(4, secondDigit);
    } else if ((counterCount > 99) && (counterCount < 1000)) { // 99-999
      int firstDigit = counterCount / 100;
      int secondDigit = (counterCount % 100) / 10;
      int thirdDigit = counterCount % 10;

      SetNumber(2, firstDigit);
      SetNumber(3, secondDigit);
      SetNumber(4, thirdDigit);
    } else if ((counterCount > 999) && (counterCount < 10000)) { // 999-9999
      int firstDigit = counterCount / 1000;
      int secondDigit = (counterCount % 1000) / 100;
      int thirdDigit = (counterCount % 100) / 10;
      int fourthDigit = counterCount % 10;

      SetNumber(1, firstDigit);
      SetNumber(2, secondDigit);
      SetNumber(3, thirdDigit);
      SetNumber(4, fourthDigit);
    }
    counterCount++;
    return;
  }
  return;
}

void printCountDown()
{
  if (countDownCount >= 0)
  {
    if (countDownCount < 10)
    { // 0-9
      SetNumber(4, countDownCount);
    }
    else if ((countDownCount > 9) && (countDownCount < 100))
    { // 9-99
      int firstDigit = countDownCount / 10;
      int secondDigit = countDownCount % 10;

      SetNumber(3, firstDigit);
      SetNumber(4, secondDigit);
    }
    else if ((countDownCount > 99) && (countDownCount < 1000))
    { // 99-999
      int firstDigit = countDownCount / 100;
      int secondDigit = (countDownCount % 100) / 10;
      int thirdDigit = countDownCount % 10;

      SetNumber(2, firstDigit);
      SetNumber(3, secondDigit);
      SetNumber(4, thirdDigit);
    }
    else if ((countDownCount > 999) && (countDownCount < 10000))
    { // 999-9999
      int firstDigit = countDownCount / 1000;
      int secondDigit = (countDownCount % 1000) / 100;
      int thirdDigit = (countDownCount % 100) / 10;
      int fourthDigit = countDownCount % 10;

      SetNumber(1, firstDigit);
      SetNumber(2, secondDigit);
      SetNumber(3, thirdDigit);
      SetNumber(4, fourthDigit);
    }
    BlankDisplay(0);
    countDownCount--;
    return;
  }
  return;
}

