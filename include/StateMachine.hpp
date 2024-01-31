#ifndef state_machine_h
#define state_machine_h

#include "FsmEnums.hpp"
#include "Settings.hpp"
#include "Arduino.h"
#include "Sensor.hpp"
#include "Vector.h"
#include "EEPROM.hpp"
#include "SerialDebug.hpp"



class FSM
{
private:
    State currentState;
    State checkOut;
    Event addOrEdit;
    Event prevEvent;
    Event event;

    typedef Vector<Sensor> VectorS;
    VectorS sensors;
    Sensor sensor;

    unsigned short sensorSelected;

    bool nextEvent = false;
    bool pageChange = false;
    bool okPressed = false;
    unsigned short page,
    maxPage[statePagesDefined] =
            {0, 3, 4, 0,
             1, 8, 3, 1,
             1, 2, 1, 1,
             1, 1, 1},
    statePageSel;

    char stp[11] = "Setting up";
    char serialReturn[15];


    bool delSensor = false;
    int sInputError;
    int mAmps;

    char sensorName[15];
    int sensorPIN;
    float sensorV;
    int sensorR;

    int lastAdd = LOW;
    int lastPrev = LOW;
    int lastOk = LOW;

    int readingAddButton;
    int readingPrevButton;
    int readingOkButton;

    bool startUpAnimation = true;
    bool useEEPROM = true;
    bool resetEEPROM = false;
    bool dummySensors = false;


    unsigned long lastDebounceTime;

public:
    void EventHandler(Event event);
    void errorHandler(int nError);
    void stateTransition(Event nEvent);
    static int charToInt(const char* input, int& sInputError);
    void pChange();
    void okPress();
    void buttonRead();
    void Startup();
    void Loop();
private:

};


#endif