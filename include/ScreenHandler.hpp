#ifndef screen_handler_h
#define screen_handler_h
#include <LiquidCrystal_I2C.h>
#include "Arduino.h"
//#include "icons.h"
struct
{
    uint8_t x : 6;
    uint8_t y : 2;

} typedef ScreenCoord;

class ScreenHandler
{
    LiquidCrystal_I2C lcd;

    ScreenHandler();

private:
    static ScreenHandler* _instance;
public:
    static ScreenHandler* GetInstance();

    void ClearScreen();

    void Init();

    void SetCoords(ScreenCoord coords);
    void Home();

    void Write(uint8_t str);

    template<typename T>
    void PrintScreen(T str);

    template<typename T>
    void PrintlnScreen(T str);

    void newLine();
    void pageNumber(int p);


    void pChangeMMP1();
    void pChangeMMP2(int size, const char* name);
    void pChangeMMP3(const char* name);
    void pChangeMMP4(const char* n1, const char* n2);

    void pChangeSetP1();
    void pChangeSetP2();
    void pChangeSetP3();

    void pChangeAEP1();
    void pChangeAEP2();
    void pChangeAEP3();
    void pChangeAEP4();

    void pChangeDelP1(const char* name);
    void pChangeDelP2(int size, const char* name);
    void pChangeDelP3(const char* n1, const char* n2);


    void pChangeDel();

    void pChangeSenP1(int i, const char* name);
    void pChangeSenP2(int i, bool b);
    void pChangeSenP3(float f, bool b);
    void pChangeSenP4(float f, int i);
    void pChangeSenP5(int i);
    void pChangeSenP6();
    void pChangeSenP7();
    void pChangeSenP8(const char* name);

    void pChangeCloP1();
    void pChangeCloP2();
    void pChangeCloP3();


    void pChangeUpl();

    void pChangeDwn();

    void pChangeChkP1();
    void pChangeChkP2();

    void pChangeSName();
    void pChangeSPin();
    void pChangeSVol();
    void pChangeSRes();
    void pChangeSFc();


    void okPressSETAnim();




    byte R1[8] = {
            0B00111,
            0B00100,
            0B00111,
            0B00111,
            0B01000,
            0B01011,
            0B01010,
            0B01100
    };
    byte R2[8] = {
            0B11110,
            0B00001,
            0B11001,
            0B11001,
            0B00001,
            0B11010,
            0B00101,
            0B00011
    };
    byte settings[8] = {
            0b00000,
            0b10101,
            0b01110,
            0b11011,
            0b01110,
            0b10101,
            0b00000,
            0b00000
    };

    byte arrow[8] = {
            0B00000,
            0B00000,
            0B00100,
            0B00010,
            0B11111,
            0B00010,
            0B00100,
            0B00000
    };

};




#endif

