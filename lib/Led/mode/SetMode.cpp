
#include "Led.h"
#include "Buzzer.h"
#include "Mode.h"

String segment1mode = "hour"; //value = | hour | isdst | mday | min | mon | sec | wday | yday | year |
String segment2mode = "min";  //value = | hour | isdst | mday | min | mon | sec | wday | yday | year |

String Mode = "clock";
String colorMode = "solid";
int counterCount = 0;
int counterLimit = 120;
int countDownCount = 100;
int timeFormat = 24;

int scors1 = 0;
int scors2 = 0;

String ntpServer = "pool.ntp.org";
long gmtOffset_sec = 7 * 3600; // UTC+7
int daylightOffset_sec = 0;

int interval = 1000;
void (*mainLoop)() = &printLocalTime; // the main function contains the mode to be executed.

void setMode(String mode)
{
    myBuzzer.off();
    BlankDots();
    if (mode == "clock")
    {
        mainLoop = &printLocalTime;
        Mode = mode;
    }
    else if (mode == "counter")
    {
        mainLoop = &printCounter;
        Mode = mode;
    }
    else if (mode == "countdown")
    {
        mainLoop = &printCountDown;
        Mode = mode;
    }
    else if (mode == "scors")
    {
        mainLoop = &printScors;
        Mode = mode;
    }
    else if (mode == "CLI")
    {
        mainLoop = &CLI;
        Mode = mode;
    }
    return;
}