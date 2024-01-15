#include <string.h>
#include "Settings.hpp"
#include "Arduino.h"
#ifndef sensor_h
#define sensor_h
class Sensor{
public:
    // Default Constructor Number 1, used in the declaration of temp -> setting the vector size
    Sensor() {}

    // Default Constructor Number 2, used for testing
    Sensor(char* name);

    // Parametarized Constructor
    Sensor(char* name, int pin, float v, int r);
    // Setters
    void setName(char* name){
        strncpy(senName, name, sizeof(senName) - 1);
        senName[sizeof(senName) - 1] = '\0';
    }

    void setPIN(int pin){
        senPIN = pin;
    }

    void setV(float v){
        senV = v;
    }

    void setR(int r){
        senR = r;
    }

    // Getters
    char* getName() const{
        return senName;
    }

    int getPIN() const{
        return senPIN;
    }

    float getV() const{
        return senV;
    }

    int getR() const{
        return senR;
    }

    int getValue() const{
        return senValue;
    }

    bool checkConnection() const{ // connection status
        return false;
    }

private:
    char senName[15];
    int senPIN;
    float senV;
    int senR;
    int senValue;
};
#endif