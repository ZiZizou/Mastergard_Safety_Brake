#include "toggle_interrupt.h"


volatile int charge_time = CHARGE_TIME_MS_1;  //charge time initalized to zero and value is loaded from EEPROM according to toggle setting 
volatile int discharge_time = DISCHARGE_TIME_MS_1;    //discharge time initialized to zero and value is loaded from EEPROM according to toggle setting
volatile int toggle_select = 1;
//toggle_select value cycles between 1 and 2. On pressing button on pin-2
//toggle select value changes to the next configuration. Acts as a record
//for the program to know which flashing sequence it is on. On initialization
//toggle select takes the value of EEPROM mode select byte

const int data[CONFIGURATIONS][2] = {
    {CHARGE_TIME_MS_1, DISCHARGE_TIME_MS_1},
    {CHARGE_TIME_MS_2, DISCHARGE_TIME_MS_2}
};

/*
@Function void toggle_ISR() - Interrupt Service Routine
parameters: None
does: On seeing input high on PIN2, it loads the next configuration from EEPROM into timing variables
returns: None
*/
void toggle_ISR(){

    toggle_select++;
    if(toggle_select>CONFIGURATIONS){
        toggle_select=1;
    }
    EEPROM.update(1,toggle_select); //update mode select byte 

    charge_time = data[toggle_select-1][0];
    discharge_time = data[toggle_select-1][1];

}

/*
@Function void EEPROM_setup() 
parameters: None
does: initializes eeprom and opens a variable mode selected in EEPROM. Mode selected variable is available in byte 1
of eeprom. Byte 0 of eeprom details whether mode select variable has been initialized. If Byte 0 is equal to 0, mode selected
bit is initialized to selection 0. If Byte 0 is equal to 1, data in Byte 1 is stored in toggle select.
By using 16 bits for storing charge times, we can use larger charge times if required
returns: None
*/
void EEPROM_setup(){

    int dataWritten = EEPROM.read(0);   //dataWritten is 1 if charge-discharge times have been already
    //written to EEPROM and 0 if not
    if(dataWritten!=1){

        EEPROM.write(1,1);  //initialize mode select to 1 for selection 1

        Serial.println("NEW DATA WRITTEN");
        EEPROM.write(0,1);  //writing dataWritten byte of EEPROM to 1 to not overwrite the data 
    }
    else{
        Serial.println("Data has already been written");
        toggle_select = EEPROM.read(1); //read mode from last time of storage into 
    }

}