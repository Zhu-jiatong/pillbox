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
        ACK,
        IDLE
    };
    struct alarms
    {
        unsigned long current{};
        long target{};
        states state{IDLE};
    } alarmDat[noOfAlarm + 1];
    unsigned long rawMillis{};
    short minIndx{}, prevIndx{}, noLeft{};
    void set(short indx, short h, short m);
    String toStr(unsigned long milli);
    unsigned long toMillis(short h, short m);
    void update(short indx);
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
        alarmDat[indx].state = tempTarget ? RUN : IDLE;
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
    String hh(h);
    if (h < 10)
        hh = "0" + hh;
    String mm(m);
    if (m < 10)
        mm = "0" + mm;
    String ss(s);
    if (s < 10)
        ss = "0" + ss;
    return hh + ":" + mm + ":" + ss;
}

void alarmClock::update(short indx)
{
    switch (indx)
    {
    case RTCINDX: // update RTC time, use index 0
    {
        unsigned long temp(millis());
        rawMillis = temp % DAYMILLIS;
        alarmDat[RTCINDX].current = (temp + alarmDat[RTCINDX].target) % DAYMILLIS;
    }
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
    unsigned long extrem(DAYMILLIS); // store second closest alarm time
    minIndx = prevIndx = noLeft = 0; // initialise 2 vars, will remain 0 if no active alarms present

    for (short i = 1; i < arrElem(alarmDat); ++i) // 1st for-loop: find closest alarm to run
        if (alarmDat[i].state == RUN)
        {
            ++noLeft;
            if (alarmDat[i].target < extrem)
            {
                extrem = alarmDat[i].target;
                minIndx = i;
            }
        }
    extrem = 0;
    for (short i = 1; i < arrElem(alarmDat); ++i) // 2nd for-loop: find previously expired alarm
        if (alarmDat[i].target > extrem && alarmDat[i].state != RUN)
        {
            extrem = alarmDat[i].target;
            prevIndx = i;
        }
}

bool alarmClock::isExpire()
{
    for (short i = 1; i < arrElem(alarmDat); ++i)
        if (alarmDat[i].state == EXPI)
            return true;
    return false;
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
