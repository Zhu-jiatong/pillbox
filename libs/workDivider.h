#if !defined(WDIVIDER_h)
#define WDIVIDER_h
#include <Arduino.h>

class workDivider
{
public:
    short cycleCount{}, cycleNo{};
    void doWork();
} mTask;

void workDivider::doWork()
{
    ++cycleNo;
}
#endif // WDIVIDER_h
