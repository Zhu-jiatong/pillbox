#if !defined(WDIVIDER_h)
#define WDIVIDER_h
#include <Arduino.h>

#include "display.h"
#include "blinkLed.h"

constexpr short threadNo(5);

class workDivider
{
public:
    short cycleNo{};
    void doWork();
} mTask;

void workDivider::doWork()
{
    cycleNo = cycleNo > threadNo ? 0 : cycleNo;
    switch (cycleNo)
    {
    case 0:
        alarm.update(RTCINDX);
        break;

    case 1:
        alarm.scanRefresh();
        break;

    case 2:
        if (alarm.isExpire())
            led.update();
        else
            led.cleanUp();
        break;

    case 3:
        alarm.update(alarm.minIndx);
        break;

    case 4:
        alarm.sort();
        break;

    case 5:
        screen.refresh();
        break;

    default:
        break;
    }
    ++cycleNo;
}
#endif // WDIVIDER_h
