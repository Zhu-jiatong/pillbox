#if !defined(WDIVIDER_h)
#define WDIVIDER_h
#include <Arduino.h>

#include "display.h"
#include "blinkLed.h"

class workDivider
{
public:
    short cycleNo{};
    void doWork();
} mTask;

void workDivider::doWork()
{
    cycleNo = cycleNo > 3 ? 0 : cycleNo;
    switch (cycleNo)
    {
    case 0:
        alarm.update(RTCINDX);
        break;

    case 1:
        if (alarm.isExpire())
            led.update();
        else
            led.cleanUp();

        break;

    case 2:
        alarm.update(alarm.minIndx);
        break;

    case 3:
        screen.refresh();
        break;

    default:
        break;
    }
    ++cycleNo;
}
#endif // WDIVIDER_h
