#include "WifiFunc.h"
#include "Led.h"

void printIpAddressToDisplay(IPAddress ip)
{
    BlankDots();
    Serial.print("Ip address : ");
    Serial.println(ip);
    // print ip address to display
    for (int i = 0; i < 4; i++)
    {
        int octet = (int)ip[i];
        BlankDisplay(50);
        PrintNumber(octet, "full");
        delay(1500);
    }

    BlankDisplay(0);
    return;
}

