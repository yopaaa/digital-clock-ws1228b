#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <EEPROM.h>
#include <time.h>
#include <FastLED.h>
#include <Wire.h>
#include <FS.h>
#include <SPIFFS.h>

#define LED_TYPE WS2812B
#define NUMBER_SEGMENT 4
#define LEDS_PER_SEGMENT 7
#define NUM_LEDS 28
#define LED_PIN 13
#define DOTS_PIN 12
#define NUM_DOTS 2

String CodeVersion = "v1.3.1";
// VARIABLE FOR WIFI AP
String ssid = "";
String password = "";
const char *APssid = "digital_clock";
const char *APpassword = "12345678zxcvbnm";
IPAddress local_ip(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

AsyncWebServer server(80);
const int SSID_ADDRESS = 400;
const int PASSWORD_ADDRESS = 432;

const int RED_ADDRESS = 101;
const int GREEN_ADDRESS = 102;
const int BLUE_ADDRESS = 103;
const int TIME_FORMAT_ADDRESS = 120;

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 7 * 3600; // UTC+7
// const long gmtOffset_sec = 7 * -3600;  // UTC-5
// const long gmtOffset_sec = 0;  // UTC+0
const int daylightOffset_sec = 3600;

CRGB leds[NUM_LEDS];
CRGB Dots[NUM_DOTS];

const int Frame[][7] = {
    {1, 1, 1, 1, 1, 1, 0}, // Number 0
    {0, 0, 1, 1, 0, 0, 0}, // Number 1
    {0, 1, 1, 0, 1, 1, 1}, // Number 2
    {0, 1, 1, 1, 1, 0, 1}, // Number 3
    {1, 0, 1, 1, 0, 0, 1}, // Number 4
    {1, 1, 0, 1, 1, 0, 1}, // Number 5
    {1, 1, 0, 1, 1, 1, 1}, // Number 6
    {0, 1, 1, 1, 0, 0, 0}, // Number 7
    {1, 1, 1, 1, 1, 1, 1}, // Number 8
    {1, 1, 1, 1, 1, 0, 1}, // Number 9
    {0, 0, 0, 0, 0, 0, 0}, // Digit off
    {1, 1, 0, 0, 1, 1, 1}, // Alfabet E
    {1, 1, 0, 0, 0, 1, 0}, // Alfabet r
    {0, 0, 0, 0, 1, 0, 0}, // Simbol _
};

byte RED = 255;
byte GREEN = 0;
byte BLUE = 0;
byte BRIGHTNESS = 100;

int displayMode = 1;
int RefreshDelay = 1000;
int counterCount = 0;
int counterLimit = 60;
int countDownCount = 100;
int timeFormat = 24;
bool isBlink = true;
const int startWifiApBtn = 5;
bool LED2_State = false;

// SETUP FUNCTION
void setup()
{
  Serial.begin(9600);
  EEPROM.begin(512);
  SPIFFS.begin();
  Wire.begin();
  initPin();
  FastLED.addLeds<LED_TYPE, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.addLeds<LED_TYPE, DOTS_PIN, GRB>(Dots, NUM_DOTS);
  FastLED.setBrightness(BRIGHTNESS); // SET BRIGHTNESS FROM EEPROM VALUE

  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
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
  listDir("/", 0);
  startWifi(0); // STA WIFI
  httpHandler();
}

void initPin(){
  pinMode(2, OUTPUT);
  pinMode(startWifiApBtn, INPUT);
  return;
}

void defaultState()
{
  // EEPROM.put(BRIGHTNESS_ADDRESS, 100);  // SET BRIGHTNESS
  EEPROM.put(RED_ADDRESS, 255); // RED COLOR
  EEPROM.put(GREEN_ADDRESS, 0); // GREEN COLOR
  EEPROM.put(BLUE_ADDRESS, 0);  // BLUE COLOR
  EEPROM.put(TIME_FORMAT_ADDRESS, 24);  // BLUE COLOR

  EEPROM.put(1, 1); // Setup first runing
  EEPROM.commit();
  Serial.println("all state is been reset...");
}

// WIFI FUNCTION
void saveWifiCredentials(const char *ssid, const char *password) {
  // Save the SSID to EEPROM
  int i = 0;
  for (; i < strlen(ssid); i++) {
    EEPROM.write(SSID_ADDRESS + i, ssid[i]);
  }
  for (; i < 32; i++) {
    EEPROM.write(SSID_ADDRESS + i, 0);
  }

  // Save the password to EEPROM
  i = 0;
  for (; i < strlen(password); i++) {
    EEPROM.write(PASSWORD_ADDRESS + i, password[i]);
  }
  for (; i < 64; i++) {
    EEPROM.write(PASSWORD_ADDRESS + i, 0);
  }

  // Commit the changes to EEPROM
  EEPROM.commit();
}

void startWifi(int wifiMode){
  if (wifiMode == 1)
  { // wifi access point mode if mode = 1
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(local_ip, gateway, subnet);
    WiFi.softAP(APssid, APpassword);

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

// WEB SERVER FUNCTION
void httpHandler()
{
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/build/index.html", "text/html"); 
  });
  server.on("/color", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/build/index.html", "text/html"); 
  });
  server.on("/mode", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/build/index.html", "text/html"); 
  });
  server.on("/setting", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/build/index.html", "text/html"); 
  });
  server.serveStatic("/", SPIFFS, "/build");

  server.on("/listdir", HTTP_GET, [](AsyncWebServerRequest *request) {
    String q = request->arg("q");
    const char *qDir = q.c_str();
    listDir(qDir, 1);
    request->send(200, "application/json", "{}"); 
  });

  server.on("/resetall", HTTP_POST, [](AsyncWebServerRequest *request) {
    defaultState();
    request->send(200, "application/json", "{}"); 
    delay(500);
    ESP.restart(); 
  });

  server.on("/wifi/set", HTTP_POST, [](AsyncWebServerRequest *request) {
    bool valuesExist = (request->hasArg("ssid")) && (request->hasArg("password"));

    if (valuesExist){
      String ssid = request->arg("ssid");
      String password = request->arg("password");

      saveWifiCredentials(ssid.c_str(), password.c_str());
      request->send(200, "application/json", "{\"status\": \"OK\"}"); 
      delay(100);
      ESP.restart();
    } else {
      request->send(400, "application/json", "{\"status\": \"Bad request\"}");
    } 
  });

  server.on("/ping", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", "<h1>PONG</h1>"); 
  });

  server.on("/led2", HTTP_POST, [](AsyncWebServerRequest *request) {
    LED2_State = !LED2_State;
    digitalWrite(2, LED2_State);
    request->send(200, "application/json", "{\"status\": \"OK\"}"); 
  });

  server.on("/color", HTTP_POST, [](AsyncWebServerRequest *request) {
    bool valuesExist = (request->hasArg("red")) && (request->hasArg("green")) && (request->hasArg("blue")) && (request->hasArg("brightness"));

    if (valuesExist){
      const int red = atoi(request->arg("red").c_str());
      const int green = atoi(request->arg("green").c_str());
      const int blue = atoi(request->arg("blue").c_str());
      const int brightness = atoi(request->arg("brightness").c_str());

      Serial.print("---------- set color red : ");
      Serial.println(red);
      Serial.print("---------- set color green : ");
      Serial.println(green);
      Serial.print("---------- set color blue : ");
      Serial.println(blue);
      Serial.print("---------- set brightness : ");
      Serial.println(brightness);

      EEPROM.put(RED_ADDRESS, red);
      EEPROM.put(GREEN_ADDRESS, green);
      EEPROM.put(BLUE_ADDRESS, blue);
      //  EEPROM.put(BRIGHTNESS_ADDRESS, brightness);
      EEPROM.commit();

      FastLED.setBrightness(brightness);
      FastLED.show();
      refreshColor();

      request->send(200, "application/json", "{\"status\": \"OK\"}"); 
    } else {
      request->send(400, "application/json", "{\"status\": \"Bad request\"}");
    } 
  });

  server.on("/timeformat", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasArg("format")){
      int mode = atoi(request->arg("format").c_str());
      timeFormat = mode;
      EEPROM.put(TIME_FORMAT_ADDRESS, mode);  // BLUE COLOR
      EEPROM.commit();
      
      request->send(200, "application/json", "{\"status\": \"OK\"}");
    } else {
      request->send(400, "application/json", "{\"status\": \"Bad request\"}");
    } 
  });

  server.on("/mode", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasArg("mode")){
      int mode = atoi(request->arg("mode").c_str());
      int limit = atoi(request->arg("limit").c_str());
      displayMode = mode;

      if (mode == 2){ // counter mode
        counterCount = 0;
        counterLimit = limit;
        BlankDots();
      } else if (mode == 3) { // count down mode
        countDownCount = limit;
        BlankDots();
      }
      
      request->send(200, "application/json", "{\"status\": \"OK\"}");
    } else {
      request->send(400, "application/json", "{\"status\": \"Bad request\"}");
    } 
  });

  server.on("/restart", HTTP_POST, [](AsyncWebServerRequest *request)  {
    request->send(200, "application/json", "{\"status\": \"OK\"}");
    delay(500);
    ESP.restart(); 
  });

  server.onNotFound([](AsyncWebServerRequest *request) {
    request->send(404, "application/json", "{\"status\": \"Not found\"}"); 
  });


  server.begin();
  Serial.println("Server started");
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

// SEGEMNT FUNCTION
void refreshColor()
{
  RED = EEPROM.read(RED_ADDRESS);
  GREEN = EEPROM.read(GREEN_ADDRESS);
  BLUE = EEPROM.read(BLUE_ADDRESS);
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
      BlankDisplay(0);
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
    countDownCount--;
    return;
  }
  return;
}

void SetNumber(int segment, int num)
{
  int x = segment * LEDS_PER_SEGMENT - LEDS_PER_SEGMENT;
  int y = 0;

  for (int i = x; i < segment * 7; i++)
  {
    if (Frame[num][y] == 1)
    {
      leds[i] = CRGB(RED, GREEN, BLUE);
    }
    else
    {
      leds[i] = CRGB(0, 0, 0);
    }
    y++;
    FastLED.show();
  }
}

void BlankDisplay(int gap)
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB(0, 0, 0);
    FastLED.show();
    delay(gap);
  }
  return;
}

void ErrorDisplay(int one, int two)
{
  SetNumber(1, 11);
  SetNumber(2, 12);
  SetNumber(3, one);
  SetNumber(4, two);
  return;
}

void TestStartUp()
{
  Serial.println("");
  Serial.print("Code Version : ");
  Serial.println(CodeVersion);
  refreshColor();
  BlankDisplay(0);
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB(RED, GREEN, BLUE);
    FastLED.show();
    delay(200);
  }
  BlankDisplay(200);
  return;
}

// DOTS FUNCTION
void BlinkDots(){
  if (isBlink == true) {
    ShowDots();
  } else {
    BlankDots();
  }
  isBlink = !isBlink;
  return;
}

void BlankDots(){
  for (int i = 0; i < NUM_DOTS; i++) {
    Dots[i] = CRGB(0, 0, 0);
  }
  return;
}

void ShowDots(){
  for (int i = 0; i < NUM_DOTS; i++) {
    Dots[i] = CRGB(RED, GREEN, BLUE);
  }
  return;
}

// SPIFFS FUNCTION
void listDir(const char *dirname, uint8_t levels)
{
  Serial.printf("Listing directory: %s\r\n", dirname);

  File root = SPIFFS.open(dirname);
  if (!root) {
    Serial.println("- failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("- not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file)
  {
    if (file.isDirectory())
    {
      Serial.print("  DIR : ");
      Serial.println(file.name());
    }
    else
    {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("\tSIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }

  Serial.println("\nSPIFFS space usage:");
  Serial.print("  Total:  ");
  Serial.println(SPIFFS.totalBytes());
  Serial.print("  Used:   ");
  Serial.println(SPIFFS.usedBytes());
  Serial.print("  Free:   ");
  Serial.println(SPIFFS.totalBytes() - SPIFFS.usedBytes());
}