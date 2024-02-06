#ifndef eeprom_hpp
#define eeprom_hpp

#include "SerialDebug.hpp"
#include "EEPROM.h"
#include "Sensor.hpp"
#include "Vector.h"

void saveToEEPROM(Vector<Sensor>* sensors);

void loadFromEEPROM(Vector<Sensor> *sensors);

void updateEEPROM(bool& delSensor, Vector<Sensor>* sensors);

#endif