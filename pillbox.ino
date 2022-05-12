#include "libs\alarmClock.h"
#include "libs\hardwareSetup.h"
#include "libs\workDivider.h"

#if defined(NODEMCU)
#include "libs\espConfig.h"
#endif // NODEMCU

void setup()
{
    hardwareInit();
    startWifi();
}

void loop()
{
    mTask.doWork();

#if defined(NODEMCU)
    dnsServer.processNextRequest();
    server.handleClient(); // Listen for HTTP requests from clients
#endif                     // NODEMCU
}