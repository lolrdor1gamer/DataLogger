#ifndef eeprom_hpp
#define eeprom_hpp
#include "SerialDebug.hpp"
#include "EEPROM.h"
#include "Sensor.hpp"
#include "Vector.h"


int numberOfSensors;

void loadFromEEPROM(Vector<Sensor>* sensors)
{
#ifdef serialDebug
    Debugger::LoadFromEEPROMDebugStart();
#endif

    Sensor sensor;
    int address = sizeof(numberOfSensors);
    EEPROM.get(0, numberOfSensors);

#ifdef serialDebug
    Debugger::LoadFromEEPROMDebugSensAmount(EEPROM.get(0, numberOfSensors));
#endif

    if(numberOfSensors < 1) numberOfSensors = 0;
    else
    {
        for(int i = 0; i < numberOfSensors; i++)
        {
            EEPROM.get(address, sensor);
            sensors->push_back(sensor);
            address += sizeof(Sensor);

            #ifdef serialDebug
            Debugger::LoadFromEEPROMDebugSensLoading(i, &(sensors->back()));
            #endif
        }
    }

#ifdef serialDebug
    Debugger::LoadFromEEPROMDebugEnd();
#endif
}

void saveToEEPROM(Vector<Sensor>* sensors)
{
#ifdef serialDebug
    Debugger::SaveToEEPROMDebugStart();
#endif


    int address = sizeof(numberOfSensors) + sizeof(Sensor) * numberOfSensors;
    numberOfSensors++;

    EEPROM.put(0, numberOfSensors);
    EEPROM.put(address, sensors->back());

#ifdef serialDebug
    Sensor sensor;
    auto a = EEPROM.get(0, numberOfSensors);
    EEPROM.get(address, sensor);

    Debugger::SaveToEEPROMDebugSensLoading(a, &sensor);
#endif

#ifdef serialDebug
    Debugger::SaveToEEPROMDebugEnd();
#endif
}



void updateEEPROM(bool& delSensor, Vector<Sensor>* sensors){
#ifdef serialDebug
    Debugger::UpdateEEPROMDebugStart();
#endif
    int address = sizeof(numberOfSensors);

    if(delSensor){
        numberOfSensors--;
        EEPROM.put(0, numberOfSensors);
        delSensor = false;
    }
#ifdef serialDebug
    Debugger::UpdateEEPROMDebugMid(EEPROM.get(0, numberOfSensors));
#endif

    if(numberOfSensors)
    {
        Sensor sensor;

        for(int i = 0; i < numberOfSensors; i++){
            sensor = sensors->at(i);
            EEPROM.put(address, sensor);
            address += sizeof(Sensor);
        }
    }

#ifdef serialDebug
    Debugger::UpdateEEPROMDebugEnd();
#endif
}



#endif