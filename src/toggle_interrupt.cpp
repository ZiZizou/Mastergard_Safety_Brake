#include "toggle_interrupt.h"


volatile int charge_time = 0;  //charge time initalized to zero and value is loaded from EEPROM according to toggle setting 
volatile int discharge_time = 0;    //discharge time initialized to zero and value is loaded from EEPROM according to toggle setting

/*
@Function void toggle_ISR() - Interrupt Service Routine
parameters: None
does: On seeing input high on PIN2, it loads the next configuration from EEPROM into timing variables
returns: None
*/
void toggle_ISR(){

    

}

/*
@Function void EEPROM_setup() 
parameters: None
does: sets up timing constants/timing sequences in the EEPROM if not setup already.
    1. Byte 0 of EEPROM indicates whether timing sequences have been written to or not.
    2. Byte 1 - first 8 bits of charge time 1 (contains most significant bit of charge time 1)
    3. Byte 2 - last 8 bits of charge time 1 (contains most significant bit of charge time 1)
    4. Byte 3 - first 8 bits of charge time 2 (contains most significant bit of charge time 2)
    5. Byte 4 - last 8 bits of charge time 2 (contains most significant bit of charge time 2)
returns: None
*/
void EEPROM_setup(){

    int dataWritten = EEPROM.read(0);   //dataWritten is 1 if charge-discharge times have been already
    //written to EEPROM and 0 if not
    if(dataWritten!=1){
        
    }

}