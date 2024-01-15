
#include "Sensor.hpp"

Sensor::Sensor(char *name)
        : senPIN(0), senV(0), senR(0), senValue(0) {
    strncpy(senName, name, sizeof(senName) - 1);
    senName[sizeof(senName) - 1] = '\0';
    if(serialDebug){
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
        : senPIN(pin), senV(v), senR(r), senValue(0) {
    strncpy(senName, name, sizeof(senName) - 1);
    senName[sizeof(senName) - 1] = '\0';
    if(serialDebug){
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
