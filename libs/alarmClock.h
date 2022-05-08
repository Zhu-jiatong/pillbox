#if !defined(ALARMCLOCK_h)
#define ALARMCLOCK_h

#include "myConfig.h"
#include <Arduino.h>

class alarmClock
{
public:
    struct alarms
    {
        ulong target{}, current{};
        short state{};
    } alarmDat[9];
    short minIndx{}, prevIndx{};
    void set(short indx, short h, short m);
    String toStr(ulong milli);
    ulong toMillis(short h, short m);
    ulong update(short indx);
    void sort();
} alarm;

void alarmClock::set(short indx, short h, short m)
{
    alarmDat[indx].target = toMillis(h, m);
}

ulong alarmClock::toMillis(short h, short m)
{
    return h * HMILLIS + m * MMILLIS;
}

String alarmClock::toStr(ulong milli)
{
    int h, m, s;

    // conversion
    h = milli / HMILLIS;
    ulong hLeft = milli % HMILLIS;
    m = hLeft / MMILLIS;
    ulong mLeft = hLeft % MMILLIS;
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

ulong alarmClock::update(short indx)
{
    switch (indx)
    {
    case RTCINDX:
        static ulong previousMillis;
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
    for (short i = 1; i < sizeof(alarmDat) / sizeof(alarmDat[0]); ++i)
    {
        short minIndex = 9;
        ulong minTime(DAYMILLIS);
        if (alarmDat[i].target < minTime)
        {
            minTime = alarmDat[i].target;
            minIndex = i;
        }
        if (minIndex != minIndx)
        {
            prevIndx = minIndx;
            minIndx = minIndex;
        }
    }
}

#endif // ALARMCLOCK_h
