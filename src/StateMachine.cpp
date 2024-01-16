#include "StateMachine.hpp"
#include "SerialDebug.hpp"

void FSM::EventHandler(Event event)
{
    nextEvent = false;
#ifdef serialDebug
Debugger::EventHandlerDebugStart(currentState);
#endif
    switch (event){
        case e_setup:
            currentState = State::SETUP;
            break;

        case e_mm:
            statePageSel = pmm;
            currentState = State::MM;
            break;

        case e_cloud:
            statePageSel = pcloud;
            currentState = State::CLOUD;
            break;

        case e_up:
            statePageSel = pupload;
            currentState = State::UPLOAD;
            break;

        case e_down:
            statePageSel = pdownload;
            currentState = State::DOWNLOAD;
            break;

        case e_cs:
            statePageSel = psensor;
            currentState = State::SENSOR;
            break;

        case e_sett:
            statePageSel = psettings;
            currentState = State::SETTINGS;
            break;

        case e_addedit:
            statePageSel = paddedit;
            currentState = State::ADDEDIT;
            break;

        case e_dellist:
            statePageSel = pdellist;
            currentState = State::DELLIST;
            break;

        case e_del:
            statePageSel = pdel;
            currentState = State::DEL;
            break;

        case e_check:
            statePageSel = pcheck;
            currentState = State::CHECK;
            break;

        case e_name:
            statePageSel = pname;
            currentState = State::s_name;
            break;

        case e_pin:
            statePageSel = ppin;
            currentState = State::s_pin;
            break;

        case e_voltage:
            statePageSel = pvoltage;
            currentState = State::s_voltage;
            break;

        case e_resistance:
            statePageSel = presistance;
            currentState = State::s_resistance;
            break;

        case e_fc:
            statePageSel = pfc;
            currentState = State::s_fc;
            break;

        default:
            errorHandler(1);
    }
#ifdef serialDebug
    Debugger::EventHandlerDebugEnd(currentState);
#endif
}


