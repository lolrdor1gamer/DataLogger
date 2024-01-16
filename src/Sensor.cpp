
#include "Sensor.hpp"

Sensor::Sensor(char *name)
        : senPIN(0), senV(0), senR(0), senValue(0)
{

    strncpy(senName, name, sizeof(senName) - 1);

    senName[sizeof(senName) - 1] = '\0';

    if(serialDebug)
    {
        Serial.println("Def. Constructor used");
        Serial.print("Name is: ");
        Serial.println(senName);
        Serial.print("PIN Used: ");
        Serial.println(senPIN);
        Serial.print("V is: ");
        Serial.println(senV);
        Serial.print("R is: ");
        Serial.println(senR);
        Serial.print("Value is: ");
        Serial.println(senValue);
        Serial.println();
    }
}

Sensor::Sensor(char *name, int pin, float v, int r)
        : senPIN(pin), senV(v), senR(r), senValue(0)
{
    strncpy(senName, name, sizeof(senName) - 1);
    senName[sizeof(senName) - 1] = '\0';
    if(serialDebug)
    {
        Serial.println("Par. Constructor used");
        Serial.print("Name is: ");
        Serial.println(senName);
        Serial.print("PIN Used: ");
        Serial.println(senPIN);
        Serial.print("V is: ");
        Serial.println(senV);
        Serial.print("R is: ");
        Serial.println(senR);
        Serial.print("Value is: ");
        Serial.println(senValue);
        Serial.println();
    }
    // V and R error checking
}

void Sensor::setName(char *name)
{
    strncpy(senName, name, sizeof(senName) - 1);
    senName[sizeof(senName) - 1] = '\0';
}

void Sensor::setPIN(int pin)
{
    senPIN = pin;
}

void Sensor::setV(float v)
{
    senV = v;
}

void Sensor::setR(int r)
{
    senR = r;
}

const char *Sensor::getName() const
{
    return senName;
}

int Sensor::getPIN() const
{
    return senPIN;
}

float Sensor::getV() const
{
    return senV;
}

int Sensor::getR() const
{
    return senR;
}

int Sensor::getValue() const
{
    return senValue;
}

bool Sensor::checkConnection() const
{ // connection status
    return false;
}