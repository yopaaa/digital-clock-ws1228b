#include <Wire.h>
#include "Var.h"
#include "HttpHandler.h"
#include "Led.h"
#include "EEPROMFunc.h"
#include "WifiFunc.h"

void setup()
{
  Serial.begin(115200);
  setupEEPROM();
  Wire.begin();
  setupLED();
  pinMode(WIFI_AP_BTN, INPUT_PULLDOWN);
  TestStartUp();
  readWifiCredentials();

  digitalRead(WIFI_AP_BTN) == HIGH ? startWifiAp() : startWifiSta();
  httpHandler();
}

void processCommand(const String &command)
{
  // Lakukan tindakan atau tanggapan terkait dengan perintah
  Serial.print("Perintah diterima: ");
  Serial.println(command);
}

// LOOP FUNCTION
void loop()
{
  // if (Serial.available())
  // {
  //   String command = Serial.readStringUntil('\n');

  //   Serial.print("-> ");
  //   Serial.println(command);
  //   if (command == "start server")
  //   {
  //     startServer();
  //   }
  //   else if (command == "test")
  //   {
  //     Serial.print("|");
  //     Serial.print("\b");
  //     delay(100);
  //     Serial.print("/");
  //     Serial.print("\b");
  //     delay(100);
  //     Serial.print("-");
  //     Serial.print("\b");
  //     delay(100);
  //     Serial.print("/");
  //     Serial.print("\b");
  //     delay(100);
  //     Serial.print("-");
  //     Serial.print("\b");
  //     delay(100);
  //     Serial.print("\\");
  //     Serial.print("\b");
  //     delay(100);
  //   }

  // }
  static String command;
  Serial.print("\r");    // atau \x1B[K
  Serial.print("-> ");   // atau \x1B[K
  Serial.print(command); // atau \x1B[K

  while (Serial.available())
  {

    char c = Serial.read();

    if (c == '\n')
    {
      if (command.length() > 0)
      {
        processCommand(command);
      }

      command = "";
    }
    else if (isPrintable(c))
    {
      command += c;
    }
  }

  // Serial.println("no command detect");
  if (displayMode == "clock")
  {
    printLocalTime();
    delay(RefreshDelay);
  }
  else if (displayMode == "counter")
  {
    printCounter();
    delay(1000);
  }
  else if (displayMode == "countdown")
  {
    printCountDown();
    delay(1000);
  }
  else
  {
    ShowDotsRgb(0, 0, 255);
  }
}
