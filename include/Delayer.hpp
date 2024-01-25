#ifndef delayer_h
#define delayer_h
#include "TimeSystem.hpp"
struct Delayer : Tickable
{
public:
    bool isCounted;
private:
    void tick()
    {
        ++counter;
        isCounted = counter > timer;
    }
public:
    Delayer(int timer) : Tickable(timer), isCounted(false)
    {
        TimeSystem::GetInstance()->AddTickable(this);
    }

};

#endif