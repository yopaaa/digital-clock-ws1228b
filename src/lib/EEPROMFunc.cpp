#include "EEPROMFunc.h"
#include <Arduino.h>
#include <EEPROM.h>
#include "../Var.h"

void defaultState()
{
    // EEPROM.put(BRIGHTNESS_ADDRESS, 100);  // SET BRIGHTNESS
    EEPROM.put(RED_ADDRESS, 255);        // RED COLOR
    EEPROM.put(GREEN_ADDRESS, 0);        // GREEN COLOR
    EEPROM.put(BLUE_ADDRESS, 0);         // BLUE COLOR
    EEPROM.put(TIME_FORMAT_ADDRESS, 24); // BLUE COLOR

    EEPROM.put(1, 1); // Setup first runing
    EEPROM.commit();
    Serial.println("all state is been reset...");
}

void saveWifiCredentials(const char *ssid, const char *password)
{
    // Save the SSID to EEPROM
    int i = 0;
    for (; i < strlen(ssid); i++)
    {
        EEPROM.write(SSID_ADDRESS + i, ssid[i]);
    }
    for (; i < 32; i++)
    {
        EEPROM.write(SSID_ADDRESS + i, 0);
    }

    // Save the password to EEPROM
    i = 0;
    for (; i < strlen(password); i++)
    {
        EEPROM.write(PASSWORD_ADDRESS + i, password[i]);
    }
    for (; i < 64; i++)
    {
        EEPROM.write(PASSWORD_ADDRESS + i, 0);
    }

    // Commit the changes to EEPROM
    EEPROM.commit();
}
