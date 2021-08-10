#include "toggle_interrupt.h"


volatile int charge_time = CHARGE_TIME_MS_1;  //charge time initalized to zero and value is loaded from EEPROM according to toggle setting 
volatile int discharge_time = DISCHARGE_TIME_MS_1;    //discharge time initialized to zero and value is loaded from EEPROM according to toggle setting
volatile int toggle_select = 1;
//toggle_select value cycles between 1 and 2. On pressing button on pin-2
//toggle select value changes to the next configuration. Acts as a record
//for the program to know which flashing sequence it is on

/*
@Function void toggle_ISR() - Interrupt Service Routine
parameters: None
does: On seeing input high on PIN2, it loads the next configuration from EEPROM into timing variables
returns: None
*/
void toggle_ISR(){

    toggle_select+=4;
    if(toggle_select>CONFIGURATIONS*4){
        toggle_select=1;
    }
    charge_time = EEPROM.read(toggle_select+1) + EEPROM.read(toggle_select)<<8;
    discharge_time = EEPROM.read(toggle_select+2) + EEPROM.read(toggle_select) <<8;


}

/*
@Function void EEPROM_setup() 
parameters: None
does: sets up timing constants/timing sequences in the EEPROM if not setup already.
    1. Byte 0 of EEPROM indicates whether timing sequences have been written to or not.
    2. Byte 1 - first 8 bits of charge time 1 (contains most significant bit of charge time 1)
    3. Byte 2 - last 8 bits of charge time 1 (contains most significant bit of charge time 1)
    4. Byte 3 - first 8 bits of discharge time 1 (contains most significant bit of discharge time 2)
    5. Byte 4 - last 8 bits of discharge time 1 (contains most significant bit of discharge time 2)
    7. Byte 5 - first 8 bits of charge time 2 (contains most significant bit of charge time 1)
    8. Byte 6 - last 8 bits of charge time 2 (contains most significant bit of charge time 1)
    9. Byte 7 - first 8 bits of discharge time 2 (contains most significant bit of discharge time 2)
    10. Byte 8 - last 8 bits of discharge time 2 (contains most significant bit of discharge time 2)
By using 16 bits for storing charge times, we can use larger charge times if required
returns: None
*/
void EEPROM_setup(){

    int dataWritten = EEPROM.read(0);   //dataWritten is 1 if charge-discharge times have been already
    //written to EEPROM and 0 if not
    if(dataWritten!=1){
        //for most significant bits of charge_time_1, we must right shift charge_time_1
        //by 8 bits and store those bits into the 1st address of EEPROM
        EEPROM.write(1, CHARGE_TIME_MS_1>>8);
        //for least significant bits of charge_time_1, we must multiply charge_time_1 with
        //0x00FF and store those bits into the 2nd address of EEPROM
        EEPROM.write(2, CHARGE_TIME_MS_1*0x00FF);
        //similarly for discharge_time_1 into the 3rd and 4th bytes
        EEPROM.write(3, DISCHARGE_TIME_MS_1>>8);
        EEPROM.write(4, DISCHARGE_TIME_MS_1*0x00FF);
        //similarly for charge_time_2 into the 5th and 6th bytes
        EEPROM.write(5, CHARGE_TIME_MS_2>>8);
        EEPROM.write(6, CHARGE_TIME_MS_2*0x00FF);
        //similarly for discharge_time_2 into the 7th and 8th bytes
        EEPROM.write(5, DISCHARGE_TIME_MS_2>>8);
        EEPROM.write(6, DISCHARGE_TIME_MS_2*0x00FF);

        Serial.println("NEW DATA WRITTEN");
        EEPROM.write(0,1);  //writing dataWritten byte of EEPROM to 1 to not overwrite the data 
    }
    else{
        Serial.println("Data has already been written");
    }

}