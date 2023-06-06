#include "Led.h"
#include "Var.h"
#include <ArduinoJson.h>

void CLI()
{
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
                Serial.print("\nComand: ");
                Serial.print(command);
                Serial.println("");
            }

            command = "";
        }
        else if (isPrintable(c))
        {
            command += c;
        }
    }
    delay(200);
}
