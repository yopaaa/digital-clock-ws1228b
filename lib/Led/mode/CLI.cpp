#include "WifiFunc.h"
#include "Storage.h"
#include "Mode.h"
#include "Led.h"

// function to handle commands from the command-line interface (CLI).
void handleCommand(String command)
{
    Serial.println("");
    String firstString = command;
    int firstSpaceIndex = firstString.indexOf(" ");
    if (firstSpaceIndex != -1)
        firstString = firstString.substring(0, firstSpaceIndex);

    if (firstString == "help")
    {
        /* code */
    }
    else if (firstString == "ifconfig")
    {
        Serial.print("Wifi ssid: ");
        Serial.print(ssid);
        Serial.print("\nWifi Password: ");
        Serial.print(password);
    }
    else if (firstString == "eeprom")
    {
        if (command == "eeprom reset")
        {
            resetEEPROM();
            Serial.println("\nSuccess!");
        }
    }
    else if (firstString == "wifi")
    {
        // Mencari posisi awal dan akhir nilai 'ssid'
        int nameStartIndex = command.indexOf("ssid=") + 5;
        int nameEndIndex = command.indexOf(" ", nameStartIndex);

        // Mencari posisi awal dan akhir nilai 'pwd'
        int pwdStartIndex = command.indexOf("pwd=") + 4;
        int pwdEndIndex = command.indexOf(" ", pwdStartIndex);

        // Mendapatkan nilai 'ssid' dan 'pwd' dari string
        String ssid = command.substring(nameStartIndex, nameEndIndex);
        String pwd = command.substring(pwdStartIndex, pwdEndIndex);

        // Menampilkan nilai 'ssid' dan 'pwd'
        Serial.println("WiFi Name: " + ssid);
        Serial.println("WiFi Password: " + pwd);

        IPAddress nullIP(0, 0, 0, 0);

        writeString(SSID_ADDRESS, ssid);
        writeString(PASSWORD_ADDRESS, pwd);
        writeStaticIp(nullIP);
    }
    else
    {
        Serial.print("Comand: ");
        Serial.print(firstString);
        Serial.print(" Not Found!");
    }

    Serial.println("");
    return;
}

void CLI()
{
    ShowDotsRgb(0, 255, 0);
    static String command;
    Serial.print("\r");
    Serial.print("-> ");
    Serial.print(command);

    while (Serial.available())
    {
        char c = Serial.read();
        if (c == '\n')
        {
            if (command.length() > 0)
                handleCommand(command);

            command = "";
        }
        else if (isPrintable(c))
            command += c;
    }
    delay(200);
}
