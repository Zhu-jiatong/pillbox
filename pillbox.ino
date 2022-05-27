#include "libs\alarmClock.h"
#include "libs\hardwareSetup.h"
#include "libs\workDivider.h"
#include "libs\webPage.h"

#if defined(NODEMCU)
#include "libs\espConfig.h"
#endif // NODEMCU

void setup()
{
    hardwareInit();
    constructWebpage();
    startWifi();
}

void loop()
{
    mTask.doWork();

#if defined(NODEMCU)
    yield();
    dnsServer.processNextRequest();
    server.handleClient(); // Listen for HTTP requests from clients
#endif                     // NODEMCU
}