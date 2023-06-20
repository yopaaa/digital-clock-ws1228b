#include "Led.h"
#include "Mode.h"

void printScors()
{
    BlinkDots();
    PrintNumber(scors1, "left");
    PrintNumber(scors2, "right");
    delay(interval);
}


