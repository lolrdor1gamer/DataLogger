#include "StateMachine.hpp"


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

void FSM::errorHandler(int nError) {
#ifdef serialDebug
    Debugger::ErrorHandlerDebug(nError);
#endif
    auto scr = ScreenHandler::GetInstance();

    scr->ClearScreen();
    scr->Home();
    scr->PrintScreen("How did we");
    scr->SetCoords({0,1});
    scr->PrintScreen("get here?");
    /*gwergerhetrh
     * remove delay();
     */
    delay(5000);
    scr->ClearScreen();
    scr->Home();
    scr->PrintScreen("ERROR NUMBER");
    scr->PrintScreen(nError);
    scr->SetCoords({0,1});
    scr->PrintScreen("Report To Timur");
    /*gwergerhetrh
 * remove delay();
 */
    delay(5000);

    stateTransition(e_mm);
}

void FSM::stateTransition(Event nEvent)
{
    prevEvent = event;
    event = nEvent;
    nextEvent = true;
    pageChange = true;
    page = 1;
}

int FSM::charToInt(const char *input, int &sInputError)
{
#ifdef serialDebug
Debugger::CharToIntDebugStart();
#endif
    float output = 0;

    for(int i = 0; input[i] != '\0'; i++){
        if(input[i] >= '0' && input[i] <= '9') output = output * 10 + (input[i] - '0');
        else if(input[i] == ',' || input[i] == '.') continue;
        else{
            sInputError = 1;
            break;
        }
    }
#ifdef serialDebug
    Debugger::CharToIntDebugEnd(sInputError);
#endif
    return output;
}

void FSM::pChange(void)
{
#ifdef serialDebug
    Debugger::PChangeStart();
#endif
    pageChange = false;

    auto scr = ScreenHandler::GetInstance();

    switch (currentState){
        case SETUP:
            okPressed = true;
            break;

        case MM:
            scr->newLine();
            if (page == 1)
            {
                scr->pChangeMMP1();
            }
            else if (page == 2)
            {
                scr->pChangeMMP2(sensors.size(), sensors.front().getName());
            }
            else if (page == maxPage[statePageSel])
            {
                scr->pChangeMMP3(sensors.back().getName());
            }
            else
            {
                scr->pChangeMMP4(sensors[page - 3].getName(), sensors[page - 2].getName());
            }
            scr->pageNumber(page);
            break;

        case SETTINGS:
            scr->newLine();
            switch (page){
                case 1:
                    scr->pChangeSetP1();
                    break;

                case 2:
                    scr->pChangeSetP2();

                    break;

                case 3:
                    scr->pChangeSetP3();

                    break;

                default:
                    errorHandler(3);
            }
            scr->pageNumber(page);
            break;

        case ADDEDIT:
            switch(page){
                case 1:
                    scr->pChangeAEP1();
                    break;

                case 2:
                    scr->pChangeAEP2();
                    break;

                case 3:
                    scr->pChangeAEP3();
                    break;

                case 4:
                    scr->pChangeAEP4();
                    break;

                default:
                    errorHandler(3);
            }
            scr->pageNumber(page);
            break;

        case DELLIST:
            scr->newLine();
            if (page == maxPage[statePageSel] - 1)
            {
                scr->pChangeDelP1(sensors.back().getName());
            }
            else if (page == maxPage[statePageSel])
            {
                scr->pChangeDelP2(sensors.size(), sensors.front().getName());
            }
            else
            {
                scr->pChangeDelP3(sensors[page-1].getName(), sensors[page].getName());
            }
            scr->pageNumber(page);
            break;

        case DEL:
            scr->pChangeDel();
            delay(500);
            okPressed = true;
            break;

        case SENSOR:
            scr->newLine();
            switch (page){
                case 1:
                    scr->pChangeSenP1(sensors[sensorSelected].getPIN(), sensors[sensorSelected].getName());
                    break;

                case 2:
                    scr->pChangeSenP2(sensors[sensorSelected].getPIN(), sensors[sensorSelected].checkConnection());
                    break;

                case 3:
                    scr->pChangeSenP3(sensors[sensorSelected].getV(), sensors[sensorSelected].checkConnection());
                    break;

                case 4:
                    scr->pChangeSenP4(sensors[sensorSelected].getV(), sensors[sensorSelected].getR());
                    break;

                case 5:
                    scr->pChangeSenP5(sensors[sensorSelected].getR());
                    break;

                case 6:
                    scr->pChangeSenP6();
                    break;

                case 7:
                    scr->pChangeSenP7();
                    break;
                    scr->pChangeSenP8(sensors[sensorSelected].getName());
                case 8:

                    break;

                default:
                    errorHandler(3);
            }
            scr->pageNumber(page);
            break;

        case CLOUD:
            scr->newLine();
            switch (page){
                case 1:
                    scr->pChangeCloP1();
                    break;

                case 2:
                    scr->pChangeCloP2();
                    break;

                case 3:
                    scr->pChangeCloP3();
                    break;

                default:
                    errorHandler(3);
            }
            scr->pageNumber(page);
            break;

        case UPLOAD:
            scr->pChangeUpl();
            delay(500);
            okPressed = true;
            break;

        case DOWNLOAD:
            scr->pChangeDwn();
            delay(500);
            okPressed = true;
            break;

        case CHECK:
            scr->newLine();
            switch(page){
                case 1:
                    scr->pChangeChkP1();
                    break;

                case 2:
                    scr->pChangeChkP2();
                    break;

                default:
                    errorHandler(3);
            }
            break;

        case s_name:
            scr->pChangeSName();
            okPressed = true;
            break;

        case s_pin:
            scr->pChangeSPin();
            okPressed = true;
            break;

        case s_voltage:
            scr->pChangeSVol();
            okPressed = true;
            break;

        case s_resistance:
            scr->pChangeSRes();
            okPressed = true;
            break;

        case s_fc:
            scr->pChangeSFc();
            delay(500);
            okPressed = true;
            break;

        default:
            errorHandler(2);
    }
}

void FSM::okPress()
{
#ifdef serialDebug
Serial.println("okPress active");
#endif
    okPressed = false;

    auto scr = ScreenHandler::GetInstance();

    switch (currentState){
        case SETUP:
            scr->Home();
            if(startUpAnimation){
                for(int i = 0; i < 10; i++){
                    scr->Write(stp[i]);
                    delay(50);
                }
            }

            if(startUpAnimation) scr->PrintScreen(".");
            if(useEEPROM){
                delay(500);

                if(resetEEPROM){
#ifdef serialDebug
                    Serial.println("\nresetEEPROM() IN");
#endif
                    for(int i = 0; i < sizeof(int) + sizeof(Sensor) * SENSORS_MAX; i++) EEPROM.write(i, 0);
#ifdef serialDebug
                    Serial.println("resetEEPROM() OUT\n");
#endif
                }
                else loadFromEEPROM(&sensors);
            }

            if(startUpAnimation) scr->PrintScreen(".");
            if(dummySensors){
                sensors.push_back(Sensor("Test1"));
                sensor = sensors[0];
                sensors.push_back(Sensor("Test2", 1, 10, 470));
                sensors.push_back(Sensor("Test3"));
                sensors.push_back(Sensor("Test4", 2, 20, 4700));
                sensors[0] = sensor;
            }

            if(startUpAnimation) scr->PrintScreen(".");
            maxPage[pmm] = sensors.size() + 2;
            maxPage[pdellist] = sensors.size() + 1;

            if(startUpAnimation) scr->PrintScreen(".");
#ifdef serialDebug
                if(!sensors.empty())
                {
                    Serial.print("Vector has ");
                    Serial.print(sensors.size());
                    Serial.println(" elements!");
                }
                else Serial.println("Vector is empty!");
#endif

            if(startUpAnimation)
            {
                scr->okPressSETAnim();
                delay(2000);
            }

            stateTransition(e_mm);
            break;

        case MM:
            if (page == 1){
                //Settings
                stateTransition(e_sett);
            }
            else if (page == 2){
                //Cloud
                stateTransition(e_cloud);
            }
            else if (page == maxPage[statePageSel])
            {
                sensorSelected = sensors.size() - 1; //Last Sensor
                stateTransition(e_cs);
#ifdef serialDebug
                Serial.print("\nsensorSelected = ");
                Serial.println(sensorSelected);
                Serial.print("AKA ");
                Serial.println(sensors[sensorSelected].getName());
                Serial.println(sensors[sensorSelected].getPIN());
                Serial.println(sensors[sensorSelected].getR());
                Serial.println(sensors[sensorSelected].getV());
                Serial.println(sensors[sensorSelected].getValue());
#endif
            }
            else
            {
                sensorSelected = page - 3; //Sensor X
                stateTransition(e_cs);
#ifdef serialDebug
                Serial.print("\nsensorSelected = ");
                Serial.println(sensorSelected);
                Serial.print("AKA ");
                Serial.println(sensors[sensorSelected].getName());
                Serial.println(sensors[sensorSelected].getPIN());
                Serial.println(sensors[sensorSelected].getR());
                Serial.println(sensors[sensorSelected].getV());
                Serial.println(sensors[sensorSelected].getValue());
#endif
            }
            break;

        case SETTINGS:
            switch (page){
                case 1:
                    //Add Sensor
                    stateTransition(e_addedit);
                    break;

                case 2:
                    //Sensor Delete List
                    stateTransition(e_dellist);
                    break;

                case 3:
                    //Back to MM
                    stateTransition(e_mm);
                    break;

                default:
                    errorHandler(5);
            }
            break;

        case ADDEDIT:
            //AddEdit Logic
            switch (page){
                case 1:
                    break;

                case 2:
                    break;

                case 3:
                    //Check
                    addOrEdit = prevEvent;
                    checkOut = State::ADDEDIT;
                    stateTransition(e_check);
                    break;

                case 4:
                    //Back to Previous
                    if(prevEvent == Event::e_check) stateTransition(addOrEdit);
                    else stateTransition(prevEvent);
                    break;

                default:
                    errorHandler(5);
            }
            break;

        case DELLIST:
            if (page == maxPage[statePageSel]){
                //Back to Sett
                stateTransition(e_sett);
            }
            else{
                //Check
                sensorSelected = page - 1;
                stateTransition(e_check);
                checkOut = State::DELLIST;
            }
            break;

        case DEL:
            maxPage[pmm]--;
            maxPage[pdellist]--;
            sensors[sensorSelected] = sensors.back();
            sensors.pop_back();
            delSensor = true;
            if(useEEPROM) updateEEPROM(delSensor, &sensors);
            //Back
            if(checkOut == DELLIST) stateTransition(e_dellist);
            else if(checkOut == SENSOR) stateTransition(e_mm);
            else errorHandler(8);
            break;

        case SENSOR:
            switch (page){
                case 1:
                    //Name
                    checkOut = State::s_name;
                    stateTransition(e_check);
                    break;

                case 2:
                    //Pin
                    checkOut = State::s_pin;
                    stateTransition(e_check);
                    break;

                case 3:
                    //Status
                    break;

                case 4:
                    //Voltage
                    checkOut = State::s_voltage;
                    stateTransition(e_check);
                    break;

                case 5:
                    //Resistance
                    checkOut = State::s_resistance;
                    stateTransition(e_check);
                    break;

                case 6:
                    //Edit
                    stateTransition(e_addedit);
                    break;

                case 7:
                    //Delete
                    checkOut = State::SENSOR;
                    stateTransition(e_check);
                    break;

                case 8:
                    //Back to MM
                    stateTransition(e_mm);
                    break;

                default:
                    errorHandler(5);
            }
            break;

        case CLOUD:
            switch (page){
                case 1:
                    //Download
                    stateTransition(e_check);
                    checkOut = State::DOWNLOAD;
                    break;

                case 2:
                    //Upload
                    stateTransition(e_check);
                    checkOut = State::UPLOAD;
                    break;

                case 3:
                    //Back to MM
                    stateTransition(e_mm);
                    break;

                default:
                    errorHandler(5);
            }
            break;

        case UPLOAD:
            //Upload Logic
            stateTransition(e_cloud);
            break;

        case DOWNLOAD:
            //Download Logic
            stateTransition(e_cloud);
            break;

        case CHECK:
            switch (page){
                case 1:
                    //Confirm
                    switch (checkOut){
                        case DOWNLOAD:
                            //Download
                            stateTransition(e_down);
                            break;

                        case UPLOAD:
                            //Upload
                            stateTransition(e_up);
                            break;

                        case ADDEDIT:
                            //AddEdit
                            Debugger::serialFlush();
                            stateTransition(e_name);
                            break;

                        case DELLIST:
                            //Delete
                            stateTransition(e_del);
                            break;

                        case SENSOR:
                            //Delete
                            stateTransition(e_del);
                            break;

                        case s_name:
                            //Edit Name
                            stateTransition(e_name);
                            break;

                        case s_pin:
                            //Edit PIN
                            stateTransition(e_pin);
                            break;

                        case s_voltage:
                            //Edit Voltage
                            stateTransition(e_voltage);
                            break;

                        case s_resistance:
                            //Edit Resistance
                            stateTransition(e_resistance);
                            break;

                        default:
                            errorHandler(6);
                    }
                    break;

                case 2:
                    //Decline
                    stateTransition(prevEvent);
                    break;

                default:
                    errorHandler(5);
            }
            break;

        case s_name:
            if(Debugger::serialCheck(&okPressed)){
                Debugger::readSerial(serialReturn, sInputError);
                strncpy(sensorName, serialReturn, sizeof(serialReturn));
#ifdef serialDebug
                    Serial.print("Name is: ");
                    Serial.println(sensorName);
#endif
                //Next substate or Return
                if(checkOut == s_name){
                    if(sInputError){
                        stateTransition(e_fc);
                        break;
                    }
                    else{
                        sensors[sensorSelected].setName(sensorName);
                        if(useEEPROM) updateEEPROM(delSensor, &sensors);
                        stateTransition(e_cs);
                        break;
                    }
                }

                if(sInputError) stateTransition(e_fc);
                else stateTransition(e_pin);
            }
            break;

        case s_pin:
            if(Debugger::serialCheck(&okPressed)){
                Debugger::readSerial(serialReturn, sInputError);
                sensorPIN = charToInt(serialReturn, sInputError);
#ifdef serialDebug
                    Serial.print("PIN is: ");
                    Serial.println(sensorPIN);
#endif
                //Error Checking
                for(int i = 0; i < sensors.size(); i++){
                    if(sensorPIN == sensors[i].getPIN()){
                        sInputError = 2;
                        break;
                    }
                }
                //Next substate or Return
                if(checkOut == s_pin){
                    if(sInputError){
                        stateTransition(e_fc);
                        break;
                    }
                    else{
                        sensors[sensorSelected].setPIN(sensorPIN);
                        if(useEEPROM) updateEEPROM(delSensor, &sensors);
                        stateTransition(e_cs);
                        break;
                    }
                }

                if(sInputError){
                    sInputError = 2;
                    stateTransition(e_fc);
                }
                else stateTransition(e_voltage);
            }
            break;

        case s_voltage:
            if(Debugger::serialCheck(&okPressed)){
                Debugger::readSerial(serialReturn, sInputError);
                sensorV = charToInt(serialReturn, sInputError) / 10;
#ifdef serialDebug
                    Serial.print("V is: ");
                    Serial.println(sensorV);
#endif
                //Error Checking
                if(sensorV > 15) sInputError = 3;
                //Next sub-state or Return
                if(checkOut == s_voltage){
                    if(sInputError){
                        stateTransition(e_fc);
                        break;
                    }
                    else{
                        stateTransition(e_resistance);
                        break;
                    }
                }

                if(sInputError){
                    sInputError = 3;
                    stateTransition(e_fc);
                }
                else stateTransition(e_resistance);
            }
            break;

        case s_resistance:
            if(Debugger::serialCheck(&okPressed)){
                Debugger::readSerial(serialReturn, sInputError);
                sensorR = charToInt(serialReturn, sInputError);
#ifdef serialDebug
                    Serial.print("R is: ");
                    Serial.println(sensorR);
#endif
                //Error Checking
                mAmps = sensorV * 1000 / sensorR;
                if(mAmps > 200) sInputError = 4;
                //Next substate or Return
                if(checkOut == s_resistance || checkOut == s_voltage){
                    if(sInputError){
                        stateTransition(e_fc);
                        break;
                    }
                    else{
                        sensors[sensorSelected].setV(sensorV);
                        sensors[sensorSelected].setR(sensorR);
                        if(useEEPROM) updateEEPROM(delSensor, &sensors);
                        stateTransition(e_cs);
                        break;
                    }
                }

                if(sInputError) sInputError = 4;
                stateTransition(e_fc);
            }
            break;

        case s_fc:
            //Incorrect Inputs, back to first substate
            if(sInputError){
                scr->ClearScreen();
                scr->Home();

                switch (sInputError){
                    case 1:
                        scr->PrintScreen("Invalid Name");
                        break;

                    case 2:
                        scr->PrintScreen("Invalid PIN");
                        break;

                    case 3:
                        scr->PrintScreen("Invalid Voltage");
                        break;

                    case 4:
                        scr->PrintScreen("Invalid Resist.");
                        break;

                    case 5:
                        scr->PrintScreen("Too much Current");
                        break;

                    default:
                        errorHandler(9);
                }

                scr->SetCoords({0, 1});
                scr->PrintScreen("Returning");
                for(int i = 1; i <= 3; i++){
                    scr->PrintScreen(".");
                    delay(1500);
                }

                if(checkOut == s_name || checkOut == s_pin || checkOut == s_voltage || checkOut == s_resistance) stateTransition(e_cs);
                else stateTransition(e_name);

                sInputError = 0;
            }
                //Correct Inputs, back to Main Menu
            else{
                if(addOrEdit == e_cs){
#ifdef serialDebug
                    Serial.println("\nModifying Sensor");
#endif
                    sensors[sensorSelected].setName(sensorName);
                    sensors[sensorSelected].setPIN(sensorPIN);
                    sensors[sensorSelected].setV(sensorV);
                    sensors[sensorSelected].setR(sensorR);
                    if(useEEPROM) updateEEPROM(delSensor, &sensors);
                }
                else if(addOrEdit == e_sett){
#ifdef serialDebug
                    Serial.println("\nAdding Sensor");
#endif
                    sensor = sensors[0];
                    sensors.push_back(Sensor(sensorName, sensorPIN, sensorV, sensorR));
                    if(sensors.size() > 1) sensors[0] = sensor;
                    maxPage[pmm]++;
                    maxPage[pdellist]++;
                    if(useEEPROM) saveToEEPROM(&sensors);
                }
                else errorHandler(7);
                stateTransition(e_mm);
            }
            break;

        default:
            errorHandler(4);
    }
}

void FSM::buttonRead()
{
    readingAddButton = digitalRead(addButtonPin);
    readingPrevButton = digitalRead(prevButtonPin);
    readingOkButton = digitalRead(okButtonPin);

    if ((millis() - lastDebounceTime) > debounceDelay && (readingAddButton != lastAdd || readingPrevButton != lastPrev || readingOkButton != lastOk)) {
        lastDebounceTime = millis();

        lastAdd = readingAddButton;
        lastPrev = readingPrevButton;
        lastOk = readingOkButton;

        if (readingAddButton == LOW) {
#ifdef serialDebug
            Serial.println("UP/ADD Detectat");
#endif
            page++;
            if(page > maxPage[statePageSel]) page = 1;
            pageChange = true;
        }

        if (readingPrevButton == LOW) {
#ifdef serialDebug
            Serial.println("DOWN/PREV Detectat");
#endif
            page--;
            if(page == 0) page = maxPage[statePageSel];
            pageChange = true;
        }

        if (readingOkButton == LOW) {
#ifdef serialDebug
            Serial.println("OK Detectat");
#endif
            okPressed = true;
        }
    }
}

void FSM::Startup()
{
    Serial.begin(9600);
    while(!Serial);

    Serial.print("startUpAnimation: ");
    Serial.println(startUpAnimation);
    Serial.print("serialDebug: ");
    Serial.println(serialDebug);
    Serial.print("useEEPROM: ");
    Serial.println(useEEPROM);
    Serial.print("resetEEPROM: ");
    Serial.println(resetEEPROM);
    Serial.print("dummySensors: ");
    Serial.println(dummySensors);

    auto scr = ScreenHandler::GetInstance();
    scr->Init();

    pinMode(addButtonPin, INPUT_PULLUP);
    pinMode(prevButtonPin, INPUT_PULLUP);
    pinMode(okButtonPin, INPUT_PULLUP);
    pinMode(extraButtonPin, INPUT_PULLUP);


    Sensor temp[SENSORS_MAX];
    sensors.setStorage(temp);

    stateTransition(Event::e_setup);
}

void FSM::Loop()
{
        buttonRead();

        if(nextEvent) EventHandler(event);
        if(pageChange) pChange();
        if(okPressed) okPress();
}
