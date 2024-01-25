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
//  ERROR 1 - EventHandler out of bounds event
//  ERROR 2 - pChange out of bounds currentState
//  ERROR 3 - non-existent page (pChange), doesn't cover S::MM and S::DEL
//  ERROR 4 - okChange out of bounds currentState
//  ERROR 5 - non-existent page (okPress), doesn't cover S::MM and S::DEL
//  ERROR 6 - checkOut out of bounds
//  ERROR 7 - s_fc out of bounds
//  ERROR 8 - DEL transition incorrect
//  ERROR 9 - sInputError out of bounds

#include "Arduino.h"
#include "StateMachine.hpp"

FSM fsm;

void setup()
{
    fsm.Startup();
}

void loop()
{
    fsm.Loop();
}
