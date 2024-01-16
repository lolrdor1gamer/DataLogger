#ifndef state_machine_h
#define state_machine_h

#include "FsmEnums.hpp"
#include "Settings.hpp"
class FSM
{
private:
    State currentState;
    State checkOut;
    Event addOrEdit;
    Event prevEvent;
    Event event;

    bool nextEvent = false;
    bool pageChange = false;
    bool okPressed = false;



public:
    void EventHandler(Event event);

private:

};


#endif