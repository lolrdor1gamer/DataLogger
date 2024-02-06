#include "SerialDebug.hpp"
#include "arduino.h"


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

const void Debugger::ErrorHandlerDebug(int nError) {
    Serial.print("Error number ");
    Serial.print(nError);
    Serial.println(", refer to error list");
}

const void Debugger::CharToIntDebugStart() {
    Serial.println("charToInt() IN");
}

const void Debugger::CharToIntDebugEnd(int& sInputError) {
    Serial.print("sInputError : ");
    if(sInputError) Serial.println("true");
    else Serial.println("false");
    Serial.println("charToInt() OUT\n");
}

const void Debugger::LoadFromEEPROMDebugStart()
{
    Serial.println("\nloadFromEEPROM() IN");
}

const void Debugger::LoadFromEEPROMDebugSensAmount(int n)
{
    Serial.print("Number of Sensors in Memory: ");
    Serial.println(n);
}

const void Debugger::LoadFromEEPROMDebugEnd()
{
    Serial.println("loadFromEEPROM() OUT\n");
}

const void Debugger::LoadFromEEPROMDebugSensLoading(int i, Sensor* sensor)
{
    Serial.print("Sensor number: ");
    Serial.println(i);
    Serial.print("Name is: ");
    Serial.println(sensor->getName());
    Serial.print("PIN Used: ");
    Serial.println(sensor->getPIN());
    Serial.print("V is: ");
    Serial.println(sensor->getV());
    Serial.print("R is: ");
    Serial.println(sensor->getR());
    Serial.print("Value is: ");
    Serial.println(sensor->getValue());
    Serial.println();
}

const void Debugger::SaveToEEPROMDebugStart()
{
    Serial.println("saveToEEPROM() IN");
}

const void Debugger::SaveToEEPROMDebugSensLoading(int i, Sensor *sensor)
{
    Serial.print("Number of Sensors in Memory: ");
    Serial.println(i);
    Serial.print("Name is: ");
    Serial.println(sensor->getName());
    Serial.print("PIN Used: ");
    Serial.println(sensor->getPIN());
    Serial.print("V is: ");
    Serial.println(sensor->getV());
    Serial.print("R is: ");
    Serial.println(sensor->getR());
    Serial.print("Value is: ");
    Serial.println(sensor->getValue());
}

const void Debugger::SaveToEEPROMDebugEnd()
{
    Serial.println("saveToEEPROM() OUT");
}

const void Debugger::UpdateEEPROMDebugStart() {
    Serial.println("\nupdateEEPROM() IN");
}

const void Debugger::UpdateEEPROMDebugMid(int i) {
    Serial.print("Number of Sensors in Memory: ");
    Serial.println(i);
}

const void Debugger::UpdateEEPROMDebugEnd() {
    Serial.println("updateEEPROM() OUT");
}

const void Debugger::PChangeStart() {
    Serial.println("pChange active");
}

bool Debugger::serialCheck(bool* okPressed)
{
    bool read = false;
    if(Serial.available()) read = true;
    else if (okPressed != nullptr) *okPressed = true;
    return read;
}

void Debugger::readSerial(char *rtrn, int &sInputError)
{
#ifdef serialDebug
Serial.println("\nreadSerial() IN");
#endif

    int cnt = 0;
    int maxSize = 15;
    char incomingChar;

    while (Serial.available()){
        incomingChar = Serial.read();
        if (incomingChar == '\n'){
            rtrn[cnt] = '\0';
            break;
        }

        if (cnt < maxSize - 1) rtrn[cnt++] = incomingChar;
        else {
            rtrn[cnt] = '\0';
            sInputError = true;
            break;
        }
    }

#ifdef serialDebug
        Serial.print("sInputError : ");
        if(sInputError) Serial.println("true");
        else Serial.println("false");
        Serial.println("readSerial() OUT\n");
#endif
}

void Debugger::serialFlush()
{
#ifdef serialDebug
    Serial.println("\nserialFlush() IN");
#endif

    while(Serial.available()) char t = Serial.read();
#ifdef serialDebug
    Serial.println("\nserialFlush() OUT");
#endif
}

