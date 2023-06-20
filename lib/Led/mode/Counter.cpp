#include "Led.h"
#include "Buzzer.h"
#include "Alarm.h"
#include "Mode.h"

void printCounter()
{
    if (counterCount <= counterLimit)
    {
        PrintNumber(counterCount, "full");
        counterCount++;
        myBuzzer.off();
        delay(interval);
    }
    else
    {
        playAlarm(200);
    }
}


