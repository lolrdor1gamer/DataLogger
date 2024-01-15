#include <string.h>
#include "Settings.hpp"
#include "Arduino.h"
#ifndef sensor_h
#define sensor_h
class Sensor{

private:
    char senName[15]{};
    int senPIN;
    float senV;
    int senR;
    int senValue;

public:
    // Default Constructor Number 1, used in the declaration of temp -> setting the vector size
    Sensor() {}
    // Default Constructor Number 2, used for testing
    Sensor(char* name);
    // Parametarized Constructor
    Sensor(char* name, int pin, float v, int r);
    // Setters
    void setName(char* name);

    void setPIN(int pin);

    void setV(float v);

    void setR(int r);

    // Getters
    const char* getName() const;

    int getPIN() const;

    float getV() const;

    int getR() const;

    int getValue() const;

    bool checkConnection() const;
};
#endif