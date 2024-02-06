#include "EEPROM.hpp"
#include "AmountOfSensors.h"


void loadFromEEPROM(Vector<Sensor> *sensors)

{
#ifdef serialDebug
    Debugger::LoadFromEEPROMDebugStart();
#endif

    Sensor sensor;
    int address = sizeof(amountOfSensors);
    EEPROM.get(0, amountOfSensors);


#ifdef serialDebug
    Debugger::LoadFromEEPROMDebugSensAmount(EEPROM.get(0, amountOfSensors));
#endif

    if(amountOfSensors < 1) amountOfSensors = 0;
    else
    {
        for(int i = 0; i < amountOfSensors; i++)
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

void updateEEPROM(bool& delSensor, Vector<Sensor>* sensors)
{
#ifdef serialDebug
    Debugger::UpdateEEPROMDebugStart();
#endif
    int address = sizeof(amountOfSensors);

    if(delSensor){
        amountOfSensors--;
        EEPROM.put(0, amountOfSensors);
        delSensor = false;
    }
#ifdef serialDebug
    Debugger::UpdateEEPROMDebugMid(EEPROM.get(0, amountOfSensors));
#endif

    if(amountOfSensors)
    {
        Sensor sensor;

        for(int i = 0; i < amountOfSensors; i++){
            sensor = sensors->at(i);
            EEPROM.put(address, sensor);
            address += sizeof(Sensor);
        }
    }

#ifdef serialDebug
    Debugger::UpdateEEPROMDebugEnd();
#endif
}

void saveToEEPROM(Vector<Sensor>* sensors)
{
#ifdef serialDebug
    Debugger::SaveToEEPROMDebugStart();
#endif


    int address = sizeof(amountOfSensors) + sizeof(Sensor) * amountOfSensors;
    amountOfSensors++;

    EEPROM.put(0, amountOfSensors);
    EEPROM.put(address, sensors->back());

#ifdef serialDebug
    Sensor sensor;
    auto a = EEPROM.get(0, amountOfSensors);
    EEPROM.get(address, sensor);

    Debugger::SaveToEEPROMDebugSensLoading(a, &sensor);
#endif

#ifdef serialDebug
    Debugger::SaveToEEPROMDebugEnd();
#endif
}
