#ifndef serial_debug_h
#define serial_debug_h
#include "FsmEnums.hpp"

class Debugger
{
public:
    static const void EventHandlerDebugStart(State state);
    static const void EventHandlerDebugEnd(State state);
};


#endif
