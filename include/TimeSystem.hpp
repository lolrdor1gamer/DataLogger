#ifndef time_system_h
#define time_system_h

#include "Vector.h"

class Tickable
{
public:
    int counter = 0;
    int timer = 0;
    virtual void tick();

    Tickable(int timer) : counter(0), timer(timer)
    {

    }
};


struct Callbacks
{
    int size;
    Vector<Tickable*> tickable;

    Callbacks& operator +=(Tickable* t);
    Callbacks& operator -=(Tickable* t);

};


class TimeSystem
{
private:
    Callbacks callbacks;
    uint64_t time;

    static TimeSystem* _instance;

public:
    static TimeSystem* GetInstance();

    void AddTickable(Tickable* t);
    void RemoveTickable(Tickable* t);
    void Tick();
};


#endif
