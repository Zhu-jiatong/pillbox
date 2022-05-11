#include "libs\alarmClock.h"
#include "libs\hardwareSetup.h"
#include "libs\workDivider.h"

void setup()
{
    hardwareInit();
    alarm.set(RTCINDX, 0, 10);
    alarm.set(4, 4, 15);
    alarm.set(1, 0, 3);
    alarm.set(3, 5, 10);
}

void loop()
{
    mTask.doWork();
}