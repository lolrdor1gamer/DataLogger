#include "SerialDebug.hpp"
#include "Arduino.h"
const void Debugger::EventHandlerDebugStart(State state) {
    Serial.println("\neventHandler() active");
    Serial.print("IN State: ");
    Serial.println(state);
}

const void Debugger::EventHandlerDebugEnd(State state) {
    Serial.print("OUT State: ");
    Serial.println(state);
    Serial.println();
}