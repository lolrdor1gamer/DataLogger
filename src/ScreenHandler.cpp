#include "ScreenHandler.hpp"
#include "icons.h"

extern byte arrow[8];
extern byte settings[8];
extern byte R1[8];
extern byte R2[8];


ScreenHandler* ScreenHandler::_instance = nullptr;

ScreenHandler::ScreenHandler() : lcd(0x27, 16, 2)
{
_instance = this;
}

ScreenHandler* ScreenHandler::GetInstance()
{
    if(_instance == nullptr)
    {
        _instance = new ScreenHandler();
    }

    return _instance;
}

void ScreenHandler::ClearScreen()
{
    lcd.clear();
}

void ScreenHandler::Home()
{
    lcd.home();
}

void ScreenHandler::SetCoords(ScreenCoord coords)
{
    lcd.setCursor(coords.x, coords.y);
}

void ScreenHandler::newLine()
{
    lcd.clear();
    lcd.home();
    lcd.write(3);
    lcd.print(" ");
}

void ScreenHandler::pageNumber(int p)
{
    lcd.setCursor(15, 1);
    lcd.print(p);
}

void ScreenHandler::pChangeMMP1()
{
    lcd.print("Settings");
    lcd.setCursor(0, 1);
    lcd.print("Cloud");
}

void ScreenHandler::pChangeMMP2(int size, const char* name)
{
    lcd.print("Cloud");
    lcd.setCursor(0, 1);
    if(size == 0) lcd.print("No Sensors");
    else lcd.print(name); // first sensor
}

void ScreenHandler::pChangeMMP3(const char *name)
{
    lcd.print(name); // last sensor
    lcd.setCursor(0, 1);
    lcd.print("Settings");
}

void ScreenHandler::pChangeMMP4(const char *n1, const char *n2)
{
    lcd.print(n1); // sensor x
    lcd.setCursor(0, 1);
    lcd.print(n2); // sensor x+1
}

void ScreenHandler::pChangeSetP1()
{
    lcd.print("Add Sensor");
    lcd.setCursor(0, 1);
    lcd.print("Del Sensor");
}

void ScreenHandler::pChangeSetP2()
{
    lcd.print("Del Sensor");
    lcd.setCursor(0, 1);
    lcd.print("Back");
}

void ScreenHandler::pChangeSetP3()
{
    lcd.print("Back");
    lcd.setCursor(0, 1);
    lcd.print("Add Sensor");
}

void ScreenHandler::pChangeAEP1()
{
    lcd.clear();
    lcd.home();
    lcd.print("Serial Monitor");
    lcd.setCursor(0, 1);
    lcd.print("Input Only");
}

void ScreenHandler::pChangeAEP2()
{
    lcd.clear();
    lcd.home();
    lcd.print("Input Only");
    lcd.setCursor(0, 1);
    lcd.print("Continue");
}

void ScreenHandler::pChangeAEP3()
{
    newLine();
    lcd.print("Continue");
    lcd.setCursor(0, 1);
    lcd.print("Back");
}

void ScreenHandler::pChangeAEP4()
{
    newLine();
    lcd.print("Back");
    lcd.setCursor(0, 1);
    lcd.print("Serial Monitor");
}

void ScreenHandler::pChangeDelP1(const char* name)
{
    lcd.print(name); // last sensor
    lcd.setCursor(0, 1);
    lcd.print("Back");
}

void ScreenHandler::pChangeDelP2(int size, const char *name)
{
    lcd.print("Back");
    lcd.setCursor(0, 1);
    if(size == 0) lcd.print("No Sensors");
    else lcd.print(name); //first sensor
}

void ScreenHandler::pChangeDelP3(const char *n1, const char *n2)
{
    lcd.print(n1); // sensor x
    lcd.setCursor(0, 1);
    lcd.print(n2); // sensor x+1
}

void ScreenHandler::pChangeDel()
{
    newLine();
    lcd.print("Deleting...");
}

void ScreenHandler::pChangeSenP1(int i, const char* name)
{

    lcd.print(name); // Name Sensor
    lcd.setCursor(0, 1);
    lcd.print("PIN Sel: "); // Pin Selected
    lcd.print(i);
}

void ScreenHandler::pChangeSenP2(int i, bool b)
{

    lcd.print("PIN Sel: "); // Pin Selected
    lcd.print(i);
    lcd.setCursor(0, 1);
    lcd.print("Status : "); // Connection Status
    lcd.print(b);
}

void ScreenHandler::pChangeSenP3(float f, bool b)
{

    lcd.print("Status : "); // Connection Status
    lcd.print(b);
    lcd.setCursor(0, 1);
    lcd.print("V Sel: "); // Voltage Selected
    lcd.print(f);
}

void ScreenHandler::pChangeSenP4(float f, int i)
{

    lcd.print("V Sel: "); // Voltage Selected
    lcd.print(f);
    lcd.setCursor(0, 1);
    lcd.print("R Sel: "); // Resistance Selected
    lcd.print(i);
}

void ScreenHandler::pChangeSenP5(int i)
{
    lcd.print("R Sel: "); // Resistance Selected
    lcd.print(i);
    lcd.setCursor(0, 1);
    lcd.print("Edit"); // -> e_addedit
}

void ScreenHandler::pChangeSenP6()
{
    lcd.print("Edit"); // -> e_addedit
    lcd.setCursor(0, 1);
    lcd.print("Delete");
}

void ScreenHandler::pChangeSenP7()
{
    lcd.print("Delete"); // -> e_check
    lcd.setCursor(0, 1);
    lcd.print("Back");
}

void ScreenHandler::pChangeSenP8(const char* name)
{
    lcd.print("Back");
    lcd.setCursor(0, 1);
    lcd.print(name); // Name Sensor
}

void ScreenHandler::pChangeCloP1()
{
    lcd.print("Download");
    lcd.setCursor(0, 1);
    lcd.print("Upload");
}

void ScreenHandler::pChangeCloP2()
{
    lcd.print("Upload");
    lcd.setCursor(0, 1);
    lcd.print("Back");
}

void ScreenHandler::pChangeCloP3()
{
    lcd.print("Back");
    lcd.setCursor(0, 1);
    lcd.print("Download");
}

void ScreenHandler::pChangeUpl()
{
    newLine();
    lcd.print("Uploading...");
}

void ScreenHandler::pChangeDwn()
{
    newLine();
    lcd.print("Downloading...");
}

void ScreenHandler::pChangeChkP1()
{
    lcd.print("Confirm");
    lcd.setCursor(0, 1);
    lcd.print("Decline");
}

void ScreenHandler::pChangeChkP2()
{
    lcd.print("Decline");
    lcd.setCursor(0, 1);
    lcd.print("Confirm");
}

void ScreenHandler::pChangeSName()
{
    newLine();
    lcd.print("Input Name");
}

void ScreenHandler::pChangeSPin()
{
    newLine();
    lcd.print("Input PIN Used");
}

void ScreenHandler::pChangeSVol()
{
    newLine();
    lcd.print("Input V Used");
}

void ScreenHandler::pChangeSRes()
{
    newLine();
    lcd.print("Input R Used");
}

void ScreenHandler::pChangeSFc()
{
    newLine();
    lcd.print("Checking...");
}


void ScreenHandler::Write(uint8_t str)
{
    lcd.write(str);
}

void ScreenHandler::okPressSETAnim()
{
    lcd.clear();
    lcd.home();
    lcd.write(0);
    lcd.write(1);
    lcd.print("Regterschot");
    lcd.setCursor(2, 1);
    lcd.print("Racing");
}

void ScreenHandler::Init()
{
    lcd.init();
    lcd.backlight();
    lcd.createChar(0, R1);
    lcd.createChar(1, R2);
    lcd.createChar(2, settings);
    lcd.createChar(3, arrow);
}





