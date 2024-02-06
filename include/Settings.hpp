#ifndef settings_h
#define settings_h

//#define serialDebug






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
#endif