#if !defined(ALARMCLOCK_h)
#define ALARMCLOCK_h

#include "myConfig.h"
#include <Arduino.h>

class alarmClock
{
public:
    struct alarms
    {
        unsigned long target{}, current{};
        short state{};
    } alarmDat[9];
    short minIndx{}, prevIndx{};
    void set(short indx, short h, short m);
    String toStr(unsigned long milli);
    unsigned long toMillis(short h, short m);
    unsigned long update(short indx);
    void sort();
} alarm;

void alarmClock::set(short indx, short h, short m)
{
    alarmDat[indx].target = toMillis(h, m);
}

unsigned long alarmClock::toMillis(short h, short m)
{
    return h * HMILLIS + m * MMILLIS;
}

String alarmClock::toStr(unsigned long milli)
{
    int h, m, s;

    // conversion
    h = milli / HMILLIS;
    unsigned long hLeft = milli % HMILLIS;
    m = hLeft / MMILLIS;
    unsigned long mLeft = hLeft % MMILLIS;
    s = mLeft / SMILLIS;

    // construct string
    String hh = String(h, DEC);
    if (h < 10)
    {
        hh = "0" + hh;
    }
    String mm = String(m, DEC);
    if (m < 10)
    {
        mm = "0" + mm;
    }
    String ss = String(s, DEC);
    if (s < 10)
    {
        ss = "0" + ss;
    }
    return hh + ":" + mm + ":" + ss;
}

unsigned long alarmClock::update(short indx)
{
    switch (indx)
    {
    case RTCINDX:
        static unsigned long previousMillis;
        alarmDat[RTCINDX].current = alarmDat[RTCINDX].target + millis() % DAYMILLIS;
        if (alarmDat[RTCINDX].current >= DAYMILLIS)
        {
            alarmDat[RTCINDX].target = 0;
        }
        return alarmDat[RTCINDX].current;
        break;

    default:
        if (alarmDat[indx].target >= alarmDat[RTCINDX].current)
        {
            alarmDat[indx].current = alarmDat[indx].target - alarmDat[RTCINDX].current;
        }
        break;
    }
}

void alarmClock::sort()
{
    unsigned long minTime(DAYMILLIS), // store closest alarm time
        secMin(DAYMILLIS);            // store second closest alarm time
    minIndx = prevIndx = 0;           // initialise 2 vars, will remain 0 if no active alarms present

    for (short i = 1; i < arrElem(alarmDat); ++i) // 1st for-loop: find closest alarm to run
    {
        if (alarmDat[i].target != 0 && alarmDat[i].target < minTime)
        {
            minTime = alarmDat[i].target;
            minIndx = i;
        }
    }

    for (short i = 0; i < arrElem(alarmDat); ++i) // 2nd for-loop: find previously expired alarm
    {
        if (alarmDat[i].target != 0 && alarmDat[i].target < secMin && alarmDat[i].target > minTime)
        {
            secMin = alarmDat[i].target;
            prevIndx = i;
        }
    }
}

#endif // ALARMCLOCK_h
