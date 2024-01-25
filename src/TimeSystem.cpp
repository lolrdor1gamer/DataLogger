#include "TimeSystem.hpp"

Callbacks& Callbacks::operator +=(Tickable* t)
{
    tickable.push_back(t);
    ++size;
    return *this;
}

Callbacks& Callbacks::operator -=(Tickable* t)
{
    for(int i = size - 1; i >= 0; --i)
    {
        if(tickable[i] == t)
        {
            tickable.remove(i);
            --size;
        }
    }
    return *this;
}

void TimeSystem::AddTickable(Tickable* t)
{
    callbacks += t;
}
void TimeSystem::RemoveTickable(Tickable* t)
{
    callbacks -= t;
}

TimeSystem* TimeSystem::GetInstance()
{
    if(_instance == nullptr)
    {
        _instance = new TimeSystem();
    }

    return  _instance;
}
void TimeSystem::Tick()
{
    for(int i = 0; i < callbacks.size; ++i)
    {
        callbacks.tickable[i]->tick();
    }
}