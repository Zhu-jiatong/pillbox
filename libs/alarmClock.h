#if !defined(ALARMCLOCK_h)
#define ALARMCLOCK_h

#include "myConfig.h"
#include <Arduino.h>

class alarmClock
{
public:
    enum states
    {
        RUN,
        EXPI,
        ACK
    };
    struct alarms
    {
        unsigned long current{};
        long target{};
        states state{};
    } alarmDat[noOfAlarm + 1];
    unsigned long rawMillis{};
    short minIndx{}, prevIndx{};
    void set(short indx, short h, short m);
    String toStr(unsigned long milli);
    unsigned long toMillis(short h, short m);
    unsigned long update(short indx);
    void sort();
    bool isExpire();
    void ackno();
    void scanRefresh();
} alarm;

void alarmClock::set(short indx, short h, short m)
{
    unsigned long tempTarget = toMillis(h, m);
    switch (indx)
    {
    case RTCINDX:
        alarmDat[indx].target = tempTarget - rawMillis;
        break;

    default:
        alarmDat[indx].target = tempTarget;
        alarmDat[indx].state = RUN;
        break;
    }
}

unsigned long alarmClock::toMillis(short h, short m)
{
    return h * HMILLIS + m * MMILLIS;
}

String alarmClock::toStr(unsigned long milli)
{
    // conversion
    short h = milli / HMILLIS;
    unsigned long hLeft = milli % HMILLIS;
    short m = hLeft / MMILLIS;
    unsigned long mLeft = hLeft % MMILLIS;
    short s = mLeft / SMILLIS;

    // construct string
    String hh = String(h, DEC);
    if (h < 10)
        hh = "0" + hh;
    String mm = String(m, DEC);
    if (m < 10)
        mm = "0" + mm;
    String ss = String(s, DEC);
    if (s < 10)
        ss = "0" + ss;
    return hh + ":" + mm + ":" + ss;
}

unsigned long alarmClock::update(short indx)
{
    switch (indx)
    {
    case RTCINDX: // update RTC time, use index 0
        rawMillis = millis() % DAYMILLIS;
        alarmDat[RTCINDX].current = (millis() + alarmDat[RTCINDX].target) % DAYMILLIS;
        return alarmDat[RTCINDX].current;
        break;

    default:                                                    // update other alarms' time
        if (alarmDat[indx].target >= alarmDat[RTCINDX].current) // check if alarm expires
            alarmDat[indx].current = alarmDat[indx].target - alarmDat[RTCINDX].current;
        else // when the alarm expires
            alarmDat[indx].state = alarmDat[indx].state == RUN ? EXPI : alarmDat[indx].state;
        break;
    }
}

void alarmClock::sort()
{
    unsigned long minTime(DAYMILLIS), // store closest alarm time
        secMin{};                     // store second closest alarm time
    minIndx = prevIndx = 0;           // initialise 2 vars, will remain 0 if no active alarms present

    for (short i = 1; i < arrElem(alarmDat); ++i) // 1st for-loop: find closest alarm to run
        if (alarmDat[i].target && alarmDat[i].target < minTime && alarmDat[i].state == RUN)
        {
            minTime = alarmDat[i].target;
            minIndx = i;
        }

    for (short i = 1; i < arrElem(alarmDat); ++i) // 2nd for-loop: find previously expired alarm
        if (alarmDat[i].target && alarmDat[i].target > secMin && alarmDat[i].state != RUN)
        {
            secMin = alarmDat[i].target;
            prevIndx = i;
        }
}

bool alarmClock::isExpire()
{
    bool temp(false);
    for (short i = 1; i < arrElem(alarmDat); ++i)
        if (alarmDat[i].state == EXPI)
            temp = true;
    return temp;
}

void alarmClock::scanRefresh()
{
    for (short i = 1; i < arrElem(alarmDat); ++i)
        if (alarmDat[i].target >= alarmDat[RTCINDX].current)
            alarmDat[i].state = RUN;
}

#if defined(NODEMCU)
IRAM_ATTR void ackno()
{
    for (short i = 1; i < arrElem(alarm.alarmDat); ++i)
        alarm.alarmDat[i].state = alarm.alarmDat[i].state == alarm.EXPI ? alarm.ACK : alarm.alarmDat[i].state;
}
#else
void ackno()
{
    for (short i = 1; i < arrElem(alarm.alarmDat); ++i)
        alarm.alarmDat[i].state = alarm.alarmDat[i].state == alarm.EXPI ? alarm.ACK : alarm.alarmDat[i].state;
}
#endif // NODEMCU

#endif // ALARMCLOCK_h
