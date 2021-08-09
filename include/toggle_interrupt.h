#ifndef TOGGLE_INTERRUPT
#define TOGGLE_INTERRUPT


#include "toggle_interrupt.h"
#include <Arduino.h>
#include <EEPROM.h>

#define CHARGE_TIME_MS_1 250  //capacitor charging time in ms
#define DISCHARGE_TIME_MS_1 300 //capacitor discharge time in ms
#define CHARGE_TIME_MS_2 250  //capacitor charging time in ms
#define DISCHARGE_TIME_MS_2 750 //capacitor discharge time in ms

void toggle_ISR();

#endif