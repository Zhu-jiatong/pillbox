#include "libs\alarmClock.h"
#include "libs\hardwareSetup.h"
#include "libs\workDivider.h"
#include <Arduino.h>

void setup()
{
    hardwareInit();
    alarm.set(0, 0, 0);
}

void loop() {}