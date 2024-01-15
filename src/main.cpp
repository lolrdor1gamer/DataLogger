//  CAR::TEST_LCD v1.3 PATCH NOTES
//  MAJOR  //
//  SD_UI v2.3 made, present in the ZIP File
//  MINOR  //
//

//  TO DOs
//  Change to another board with more dynamic memory; vectors take up a lot of space
//  A list of all the Serial Debug Functionalities needs to be added
//  getValue() not used yet, assess if it's going to be used and where
//  checkConnection(), if possible
//  Hardware-mapping
//  GPIO Debug
//  CLOUD
//  Divide code into multiple files

//  SERIAL DEBUG FUNCTIONALITIES/SENSITIVITY-LIST
//

//  ERROR LIST
//  ERROR 1 - eventHandler out of bounds event
//  ERROR 2 - pChange out of bounds currentState
//  ERROR 3 - non-existent page (pChange), doesn't cover S::MM and S::DEL
//  ERROR 4 - okChange out of bounds currentState
//  ERROR 5 - non-existent page (okPress), doesn't cover S::MM and S::DEL
//  ERROR 6 - checkOut out of bounds
//  ERROR 7 - s_fc out of bounds
//  ERROR 8 - DEL transition incorrect
//  ERROR 9 - sInputError out of bounds
#include "Arduino.h"
#include "icons.h"
#include "Vector.h"
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include "Settings.hpp"
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define prevButtonPin 5   // K1
#define addButtonPin 6    // K2
#define okButtonPin 7     // K3
#define extraButtonPin 4  // K4
#define debounceDelay 50

#define SENSORS_MAX 8    // Maximum ammount of allowed sensors, 2^k

#define statePagesDefined 15
//some of these are not being used yet, but they're declared anyway for possible future use
//values for statePageSel
/*mP0*/ #define pmm 0
/*mP1*/ #define psettings 1
/*mP2*/ #define paddedit 2
/*mP3*/ #define pdellist 3
/*mP4*/ #define pdel 4
/*mP5*/ #define psensor 5
/*mP5*/ #define pcloud 6
/*mP6*/ #define pupload 7
/*mP7*/ #define pdownload 8
/*mP8*/ #define pcheck 9
/*mP9*/ #define pname 10
/*mP10*/#define ppin 11
/*mP11*/#define pvoltage 12
/*mP12*/#define presistance 13
/*mP13*/#define pfc 14

unsigned short sensorSelected;
unsigned short page, maxPage[statePagesDefined] = {0, 3, 4, 0, 1, 8, 3, 1, 1, 2, 1, 1, 1, 1, 1}, statePageSel;

enum State {SETUP, MM, SETTINGS, ADDEDIT, DELLIST, DEL, SENSOR, CLOUD, UPLOAD, DOWNLOAD, CHECK, s_name, s_pin, s_voltage, s_resistance, s_fc};
enum Event {e_setup, e_mm, e_cloud, e_up, e_down, e_cs, e_sett, e_addedit, e_dellist, e_del, e_check, e_name, e_pin, e_voltage, e_resistance, e_fc};
State currentState;
State checkOut;
Event addOrEdit;
Event prevEvent;
Event event;
bool nextEvent = false;
bool pageChange = false;
bool okPressed = false;

bool delSensor = false;
int sInputError;
int mAmps;

//   SETTINGS

//   END SETTINGS

char stp[] = "Setting up";
char serialReturn[15];

int lastAdd = LOW;
int lastPrev = LOW;
int lastOk = LOW;

int readingAddButton;
int readingPrevButton;
int readingOkButton;

unsigned long lastDebounceTime;

// still needs modifications
// V and R default values need modifying and maybe an enum for predefined values - checked in s_fc

//Used when creating/modifying sensors
char sensorName[15];
int sensorPIN;
float sensorV;
int sensorR;
int numberOfSensors;

typedef Vector<Sensor> VectorS;
VectorS sensors;
Sensor sensor;

void setup(){
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

    lcd.init();
    lcd.backlight();
    lcd.createChar(0, R1);
    lcd.createChar(1, R2);
    lcd.createChar(2, settings);
    lcd.createChar(3, arrow);

    pinMode(addButtonPin, INPUT_PULLUP);
    pinMode(prevButtonPin, INPUT_PULLUP);
    pinMode(okButtonPin, INPUT_PULLUP);
    pinMode(extraButtonPin, INPUT_PULLUP);

    Sensor temp[SENSORS_MAX];
    sensors.setStorage(temp);

    stateTransition(Event::e_setup);
}

void loop(){
    buttonRead();

    if(nextEvent == true) eventHandler(event);
    if(pageChange == true) pChange();
    if(okPressed == true) okPress();
}

//buttonRead is ready to go
void buttonRead(void){
    readingAddButton = digitalRead(addButtonPin);
    readingPrevButton = digitalRead(prevButtonPin);
    readingOkButton = digitalRead(okButtonPin);

    if ((millis() - lastDebounceTime) > debounceDelay && (readingAddButton != lastAdd || readingPrevButton != lastPrev || readingOkButton != lastOk)) {
        lastDebounceTime = millis();

        lastAdd = readingAddButton;
        lastPrev = readingPrevButton;
        lastOk = readingOkButton;

        if (readingAddButton == LOW) {
            if(serialDebug) Serial.println("UP/ADD Detectat");
            page++;
            if(page > maxPage[statePageSel]) page = 1;
            pageChange = true;
        }

        if (readingPrevButton == LOW) {
            if(serialDebug) Serial.println("DOWN/PREV Detectat");
            page--;
            if(page == 0) page = maxPage[statePageSel];
            pageChange = true;
        }

        if (readingOkButton == LOW) {
            if(serialDebug) Serial.println("OK Detectat");
            okPressed = true;
        }
    }
}

//eventHandler is ready to go
void eventHandler(Event event){
    nextEvent = false;
    if(serialDebug){
        Serial.println("\neventHandler() active");
        Serial.print("IN State: ");
        Serial.println(currentState);
    }

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
    if(serialDebug){
        Serial.print("OUT State: ");
        Serial.println(currentState);
        Serial.println();
    }
}

//pChange is ready to go
void pChange(void){
    if(serialDebug) Serial.println("pChange active");
    pageChange = false;

    switch (currentState){
        case SETUP:
            okPressed = true;
            break;

        case MM:
            newLine();
            if (page == 1){
                lcd.print("Settings");
                lcd.setCursor(0, 1);
                lcd.print("Cloud");
            }
            else if (page == 2){
                lcd.print("Cloud");
                lcd.setCursor(0, 1);
                if(sensors.size() == 0) lcd.print("No Sensors");
                else lcd.print(sensors.front().getName()); // first sensor
            }
            else if (page == maxPage[statePageSel]){
                lcd.print(sensors.back().getName()); // last sensor
                lcd.setCursor(0, 1);
                lcd.print("Settings");
            }
            else{
                lcd.print(sensors[page-3].getName()); // sensor x
                lcd.setCursor(0, 1);
                lcd.print(sensors[page-2].getName()); // sensor x+1
            }
            pageNumber();
            break;

        case SETTINGS:
            newLine();
            switch (page){
                case 1:
                    lcd.print("Add Sensor");
                    lcd.setCursor(0, 1);
                    lcd.print("Del Sensor");
                    break;

                case 2:
                    lcd.print("Del Sensor");
                    lcd.setCursor(0, 1);
                    lcd.print("Back");
                    break;

                case 3:
                    lcd.print("Back");
                    lcd.setCursor(0, 1);
                    lcd.print("Add Sensor");
                    break;

                default:
                    errorHandler(3);
            }
            pageNumber();
            break;

        case ADDEDIT:
            switch(page){
                case 1:
                    lcd.clear();
                    lcd.home();
                    lcd.print("Serial Monitor");
                    lcd.setCursor(0, 1);
                    lcd.print("Input Only");
                    break;

                case 2:
                    lcd.clear();
                    lcd.home();
                    lcd.print("Input Only");
                    lcd.setCursor(0, 1);
                    lcd.print("Continue");
                    break;

                case 3:
                    newLine();
                    lcd.print("Continue");
                    lcd.setCursor(0, 1);
                    lcd.print("Back");
                    break;

                case 4:
                    newLine();
                    lcd.print("Back");
                    lcd.setCursor(0, 1);
                    lcd.print("Serial Monitor");
                    break;

                default:
                    errorHandler(3);
            }
            pageNumber();
            break;

        case DELLIST:
            newLine();
            if (page == maxPage[statePageSel] - 1){
                lcd.print(sensors.back().getName()); // last sensor
                lcd.setCursor(0, 1);
                lcd.print("Back");
            }
            else if (page == maxPage[statePageSel]){
                lcd.print("Back");
                lcd.setCursor(0, 1);
                if(sensors.size() == 0) lcd.print("No Sensors");
                else lcd.print(sensors.front().getName()); //first sensor
            }
            else{
                lcd.print(sensors[page-1].getName()); // sensor x
                lcd.setCursor(0, 1);
                lcd.print(sensors[page].getName()); // sensor x+1
            }
            pageNumber();
            break;

        case DEL:
            newLine();
            lcd.print("Deleting...");
            delay(500);
            okPressed = true;
            break;

        case SENSOR:
            newLine();
            switch (page){
                case 1:
                    lcd.print(sensors[sensorSelected].getName()); // Name Sensor
                    lcd.setCursor(0, 1);
                    lcd.print("PIN Sel: "); // Pin Selected
                    lcd.print(sensors[sensorSelected].getPIN());
                    break;

                case 2:
                    lcd.print("PIN Sel: "); // Pin Selected
                    lcd.print(sensors[sensorSelected].getPIN());
                    lcd.setCursor(0, 1);
                    lcd.print("Status : "); // Connection Status
                    lcd.print(sensors[sensorSelected].checkConnection());
                    break;

                case 3:
                    lcd.print("Status : "); // Connection Status
                    lcd.print(sensors[sensorSelected].checkConnection());
                    lcd.setCursor(0, 1);
                    lcd.print("V Sel: "); // Voltage Selected
                    lcd.print(sensors[sensorSelected].getV());
                    break;

                case 4:
                    lcd.print("V Sel: "); // Voltage Selected
                    lcd.print(sensors[sensorSelected].getV());
                    lcd.setCursor(0, 1);
                    lcd.print("R Sel: "); // Resistance Selected
                    lcd.print(sensors[sensorSelected].getR());
                    break;

                case 5:
                    lcd.print("R Sel: "); // Resistance Selected
                    lcd.print(sensors[sensorSelected].getR());
                    lcd.setCursor(0, 1);
                    lcd.print("Edit"); // -> e_addedit
                    break;

                case 6:
                    lcd.print("Edit"); // -> e_addedit
                    lcd.setCursor(0, 1);
                    lcd.print("Delete");
                    break;

                case 7:
                    lcd.print("Delete"); // -> e_check
                    lcd.setCursor(0, 1);
                    lcd.print("Back");
                    break;

                case 8:
                    lcd.print("Back");
                    lcd.setCursor(0, 1);
                    lcd.print(sensors[sensorSelected].getName()); // Name Sensor
                    break;

                default:
                    errorHandler(3);
            }
            pageNumber();
            break;

        case CLOUD:
            newLine();
            switch (page){
                case 1:
                    lcd.print("Download");
                    lcd.setCursor(0, 1);
                    lcd.print("Upload");
                    break;

                case 2:
                    lcd.print("Upload");
                    lcd.setCursor(0, 1);
                    lcd.print("Back");
                    break;

                case 3:
                    lcd.print("Back");
                    lcd.setCursor(0, 1);
                    lcd.print("Download");
                    break;

                default:
                    errorHandler(3);
            }
            pageNumber();
            break;

        case UPLOAD:
            newLine();
            lcd.print("Uploading...");
            delay(500);
            okPressed = true;
            break;

        case DOWNLOAD:
            newLine();
            lcd.print("Downloading...");
            delay(500);
            okPressed = true;
            break;

        case CHECK:
            newLine();
            switch(page){
                case 1:
                    lcd.print("Confirm");
                    lcd.setCursor(0, 1);
                    lcd.print("Decline");
                    break;

                case 2:
                    lcd.print("Decline");
                    lcd.setCursor(0, 1);
                    lcd.print("Confirm");
                    break;

                default:
                    errorHandler(3);
            }
            break;

        case s_name:
            newLine();
            lcd.print("Input Name");
            okPressed = true;
            break;

        case s_pin:
            newLine();
            lcd.print("Input PIN Used");
            okPressed = true;
            break;

        case s_voltage:
            newLine();
            lcd.print("Input V Used");
            okPressed = true;
            break;

        case s_resistance:
            newLine();
            lcd.print("Input R Used");
            okPressed = true;
            break;

        case s_fc:
            newLine();
            lcd.print("Checking...");
            delay(500);
            okPressed = true;
            break;

        default:
            errorHandler(2);
    }
}

//okPress is ready to go
void okPress(void){
    if(serialDebug) Serial.println("okPress active");
    okPressed = false;

    switch (currentState){
        case SETUP:
            lcd.home();
            if(startUpAnimation){
                for(int i = 0; i < 10; i++){
                    lcd.write(stp[i]);
                    delay(50);
                }
            }

            if(startUpAnimation) lcd.print(".");
            if(useEEPROM){
                delay(500);

                if(resetEEPROM){
                    if(serialDebug) Serial.println("\nresetEEPROM() IN");
                    for(int i = 0; i < sizeof(int) + sizeof(Sensor) * SENSORS_MAX; i++) EEPROM.write(i, 0);
                    if(serialDebug) Serial.println("resetEEPROM() OUT\n");
                }
                else loadFromEEPROM();
            }

            if(startUpAnimation) lcd.print(".");
            if(dummySensors){
                sensors.push_back(Sensor("Test1"));
                sensor = sensors[0];
                sensors.push_back(Sensor("Test2", 1, 10, 470));
                sensors.push_back(Sensor("Test3"));
                sensors.push_back(Sensor("Test4", 2, 20, 4700));
                sensors[0] = sensor;
            }

            if(startUpAnimation) lcd.print(".");
            maxPage[pmm] = sensors.size() + 2;
            maxPage[pdellist] = sensors.size() + 1;

            if(startUpAnimation) lcd.print(".");
            if(serialDebug){
                if(!sensors.empty()){
                    Serial.print("Vector has ");
                    Serial.print(sensors.size());
                    Serial.println(" elements!");
                }
                else Serial.println("Vector is empty!");
            }

            if(startUpAnimation){
                lcd.clear();
                lcd.home();
                lcd.write(0);
                lcd.write(1);
                lcd.print("Regterschot");
                lcd.setCursor(2, 1);
                lcd.print("Racing");
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
            else if (page == maxPage[statePageSel]){
                sensorSelected = sensors.size() - 1; //Last Sensor
                stateTransition(e_cs);
                if(serialDebug){
                    Serial.print("\nsensorSelected = ");
                    Serial.println(sensorSelected);
                    Serial.print("AKA ");
                    Serial.println(sensors[sensorSelected].getName());
                    Serial.println(sensors[sensorSelected].getPIN());
                    Serial.println(sensors[sensorSelected].getR());
                    Serial.println(sensors[sensorSelected].getV());
                    Serial.println(sensors[sensorSelected].getValue());
                }
            }
            else{
                sensorSelected = page - 3; //Sensor X
                stateTransition(e_cs);
                if(serialDebug){
                    Serial.print("\nsensorSelected = ");
                    Serial.println(sensorSelected);
                    Serial.print("AKA ");
                    Serial.println(sensors[sensorSelected].getName());
                    Serial.println(sensors[sensorSelected].getPIN());
                    Serial.println(sensors[sensorSelected].getR());
                    Serial.println(sensors[sensorSelected].getV());
                    Serial.println(sensors[sensorSelected].getValue());
                }
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
            if(useEEPROM) updateEEPROM(delSensor);
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
                            serialFlush();
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
            if(serialCheck()){
                readSerial(serialReturn, sInputError);
                strncpy(sensorName, serialReturn, sizeof(serialReturn));
                if(serialDebug){
                    Serial.print("Name is: ");
                    Serial.println(sensorName);
                }
                //Next sub-state or Return
                if(checkOut == s_name){
                    if(sInputError){
                        stateTransition(e_fc);
                        break;
                    }
                    else{
                        sensors[sensorSelected].setName(sensorName);
                        if(useEEPROM) updateEEPROM(delSensor);
                        stateTransition(e_cs);
                        break;
                    }
                }

                if(sInputError) stateTransition(e_fc);
                else stateTransition(e_pin);
            }
            break;

        case s_pin:
            if(serialCheck()){
                readSerial(serialReturn, sInputError);
                sensorPIN = charToInt(serialReturn, sInputError);
                if(serialDebug){
                    Serial.print("PIN is: ");
                    Serial.println(sensorPIN);
                }
                //Error Checking
                for(int i = 0; i < sensors.size(); i++){
                    if(sensorPIN == sensors[i].getPIN()){
                        sInputError = 2;
                        break;
                    }
                }
                //Next sub-state or Return
                if(checkOut == s_pin){
                    if(sInputError){
                        stateTransition(e_fc);
                        break;
                    }
                    else{
                        sensors[sensorSelected].setPIN(sensorPIN);
                        if(useEEPROM) updateEEPROM(delSensor);
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
            if(serialCheck()){
                readSerial(serialReturn, sInputError);
                sensorV = charToInt(serialReturn, sInputError) / 10;
                if(serialDebug){
                    Serial.print("V is: ");
                    Serial.println(sensorV);
                }
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
            if(serialCheck()){
                readSerial(serialReturn, sInputError);
                sensorR = charToInt(serialReturn, sInputError);
                if(serialDebug){
                    Serial.print("R is: ");
                    Serial.println(sensorR);
                }
                //Error Checking
                mAmps = sensorV * 1000 / sensorR;
                if(mAmps > 200) sInputError = 4;
                //Next sub-state or Return
                if(checkOut == s_resistance || checkOut == s_voltage){
                    if(sInputError){
                        stateTransition(e_fc);
                        break;
                    }
                    else{
                        sensors[sensorSelected].setV(sensorV);
                        sensors[sensorSelected].setR(sensorR);
                        if(useEEPROM) updateEEPROM(delSensor);
                        stateTransition(e_cs);
                        break;
                    }
                }

                if(sInputError) sInputError = 4;
                stateTransition(e_fc);
            }
            break;

        case s_fc:
            //Incorrect Inputs, back to first sub-state
            if(sInputError){
                lcd.clear();
                lcd.home();

                switch (sInputError){
                    case 1:
                        lcd.print("Invalid Name");
                        break;

                    case 2:
                        lcd.print("Invalid PIN");
                        break;

                    case 3:
                        lcd.print("Invalid Voltage");
                        break;

                    case 4:
                        lcd.print("Invalid Resist.");
                        break;

                    case 5:
                        lcd.print("Too much Current");
                        break;

                    default:
                        errorHandler(9);
                }

                lcd.setCursor(0, 1);
                lcd.print("Returning");
                for(int i = 1; i <= 3; i++){
                    lcd.print(".");
                    delay(1500);
                }

                if(checkOut == s_name || checkOut == s_pin || checkOut == s_voltage || checkOut == s_resistance) stateTransition(e_cs);
                else stateTransition(e_name);

                sInputError = 0;
            }
                //Correct Inputs, back to Main Menu
            else{
                if(addOrEdit == e_cs){
                    if(serialDebug) Serial.println("\nModifying Sensor");
                    sensors[sensorSelected].setName(sensorName);
                    sensors[sensorSelected].setPIN(sensorPIN);
                    sensors[sensorSelected].setV(sensorV);
                    sensors[sensorSelected].setR(sensorR);
                    if(useEEPROM) updateEEPROM(delSensor);
                }
                else if(addOrEdit == e_sett){
                    if(serialDebug) Serial.println("\nAdding Sensor");
                    sensor = sensors[0];
                    sensors.push_back(Sensor(sensorName, sensorPIN, sensorV, sensorR));
                    if(sensors.size() > 1) sensors[0] = sensor;
                    maxPage[pmm]++;
                    maxPage[pdellist]++;
                    if(useEEPROM) saveToEEPROM();
                }
                else errorHandler(7);
                stateTransition(e_mm);
            }
            break;

        default:
            errorHandler(4);
    }
}

//readSerial is ready to go
void readSerial(char* rtrn, int& sInputError){
    if(serialDebug) Serial.println("\nreadSerial() IN");

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
        else if(incomingChar != '\n'){
            rtrn[cnt] = '\0';
            sInputError = true;
            break;
        }
    }

    if(serialDebug){
        Serial.print("sInputError : ");
        if(sInputError) Serial.println("true");
        else Serial.println("false");
        Serial.println("readSerial() OUT\n");
    }
}

//aux, used in pChange
void newLine(void){
    lcd.clear();
    lcd.home();
    lcd.write(3);
    lcd.print(" ");
}

//aux, used in pChange
void pageNumber(void){
    lcd.setCursor(15, 1);
    lcd.print(page);
}

//aux, used in okPress
void stateTransition(Event nEvent){
    prevEvent = event;
    event = nEvent;
    nextEvent = true;
    pageChange = true;
    page = 1;
}

//aux, used in okPress
int charToInt(char* input, int& sInputError){
    if(serialDebug) Serial.println("charToInt() IN");

    float output = 0;

    for(int i = 0; input[i] != '\0'; i++){
        if(input[i] >= '0' && input[i] <= '9') output = output * 10 + (input[i] - '0');
        else if(input[i] == ',' || input[i] == '.') continue;
        else{
            sInputError = 1;
            break;
        }
    }

    if(serialDebug){
        Serial.print("sInputError : ");
        if(sInputError) Serial.println("true");
        else Serial.println("false");
        Serial.println("charToInt() OUT\n");
    }

    return output;
}

//loadFromEEPROM is ready to go
void loadFromEEPROM(void){
    if(serialDebug) Serial.println("\nloadFromEEPROM() IN");

    int address = sizeof(numberOfSensors);
    EEPROM.get(0, numberOfSensors);
    if(serialDebug){
        Serial.print("Number of Sensors in Memory: ");
        Serial.println(EEPROM.get(0, numberOfSensors));
    }

    if(numberOfSensors < 1) numberOfSensors = 0;
    else{
        for(int i = 0; i < numberOfSensors; i++){
            EEPROM.get(address, sensor);
            sensors.push_back(sensor);
            Serial.print("Sensor number: ");
            Serial.println(i);
            Serial.print("Name is: ");
            Serial.println(sensors.back().getName());
            Serial.print("PIN Used: ");
            Serial.println(sensors.back().getPIN());
            Serial.print("V is: ");
            Serial.println(sensors.back().getV());
            Serial.print("R is: ");
            Serial.println(sensors.back().getR());
            Serial.print("Value is: ");
            Serial.println(sensors.back().getValue());
            Serial.println();
            address += sizeof(Sensor);
        }
    }

    if(serialDebug) Serial.println("loadFromEEPROM() OUT\n");
}

//loadFromEEPROM is ready to go
void saveToEEPROM(void){
    if(serialDebug) Serial.println("saveToEEPROM() IN");

    int address = sizeof(numberOfSensors) + sizeof(Sensor) * numberOfSensors;
    numberOfSensors++;

    EEPROM.put(0, numberOfSensors);
    EEPROM.put(address, sensors.back());

    if(serialDebug){
        Serial.print("Number of Sensors in Memory: ");
        Serial.println(EEPROM.get(0, numberOfSensors));
        EEPROM.get(address, sensor);
        Serial.print("Name is: ");
        Serial.println(sensor.getName());
        Serial.print("PIN Used: ");
        Serial.println(sensor.getPIN());
        Serial.print("V is: ");
        Serial.println(sensor.getV());
        Serial.print("R is: ");
        Serial.println(sensor.getR());
        Serial.print("Value is: ");
        Serial.println(sensor.getValue());
    }

    if(serialDebug) Serial.println("saveToEEPROM() OUT");
}

//updateEEPROM is ready to go
void updateEEPROM(bool& delSensor){
    if(serialDebug) Serial.println("\nupdateEEPROM() IN");

    int address = sizeof(numberOfSensors);
    if(delSensor == true){
        numberOfSensors--;
        EEPROM.put(0, numberOfSensors);
        delSensor = false;
    }
    if(serialDebug){
        Serial.print("Number of Sensors in Memory: ");
        Serial.println(EEPROM.get(0, numberOfSensors));
    }

    if(numberOfSensors){
        for(int i = 0; i < numberOfSensors; i++){
            sensor = sensors[i];
            EEPROM.put(address, sensor);
            address += sizeof(Sensor);
        }
    }

    if(serialDebug) Serial.println("updateEEPROM() OUT");
}

//serialFlush is ready to go
void serialFlush(void){
    if(serialDebug) Serial.println("\nserialFlush() IN");

    while(Serial.available()) char t = Serial.read();

    if(serialDebug) Serial.println("\nserialFlush() OUT");
}

//aux, used in okPress
bool serialCheck(void){
    bool read = false;

    if(Serial.available()) read = true;
    else okPressed = true;

    return read;
}

//in case an error comes up, this handles it
void errorHandler(int nError){
    if(serialDebug){
        Serial.print("Error number ");
        Serial.print(nError);
        Serial.println(", refer to error list");
    }
    lcd.clear();
    lcd.home();
    lcd.print("How did we");
    lcd.setCursor(0, 1);
    lcd.print("get here?");
    delay(5000);
    lcd.clear();
    lcd.home();
    lcd.print("ERROR NUMBER ");
    lcd.print(nError);
    lcd.setCursor(0, 1);
    lcd.print("Report to TIMUR");
    delay(5000);
    stateTransition(e_mm);
}