
#ifndef serial_debug_h
#define serial_debug_h
#include "FsmEnums.hpp"
#include "Sensor.hpp"
#include "Arduino.h"

class Debugger {
public:
    static const void EventHandlerDebugStart(State state);

    static const void EventHandlerDebugEnd(State state);

    static const void ErrorHandlerDebug(int nError);

    static const void CharToIntDebugStart();

    static const void CharToIntDebugEnd(int &sInputError);

    static const void LoadFromEEPROMDebugStart();

    static const void LoadFromEEPROMDebugSensAmount(int n);

    static const void LoadFromEEPROMDebugSensLoading(int i, Sensor *sensor);

    static const void LoadFromEEPROMDebugEnd();

    static const void SaveToEEPROMDebugStart();

    static const void SaveToEEPROMDebugSensLoading(int i, Sensor *sensor);

    static const void SaveToEEPROMDebugEnd();

    static const void UpdateEEPROMDebugStart();

    static const void UpdateEEPROMDebugMid(int i);

    static const void UpdateEEPROMDebugEnd();

    static const void PChangeStart();

    static bool serialCheck(bool *okPressed);

    static void readSerial(char *rtrn, int &sInputError);

    static void serialFlush();

};

#endif
