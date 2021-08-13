#include "Wire.h"
#include <Arduino.h>
#include "helper_functions.h"
#include "toggle_interrupt.h"

//#include "SPI.h"

LSM6DSO myIMU; //Default constructor is I2C, addr 0x6B
//Arduino UNO pins for I2C are A4 (SDA) and A5 (SCL)
float prev; //stores previous value of acceleration to be compared with current value from LSM6DSO




void setup() {

  Serial.begin(9600);
  pinMode(PIN7, OUTPUT);  //set relay1_switch pin to output
  pinMode(PIN6, OUTPUT);  //set relay2_switch pin to output 
  pinMode(PIN2, INPUT); //set pin-2 to be flashing mode selector 
  Serial.println("FLASHING PINS SET");

  //EEPROM_setup(); //perform eeprom writes for different configurations if not already done

  //delay(500); 
  Serial.println("PRE IMU SETUP");
  IMUConnect(&myIMU);
  Serial.println("POST IMU SETUP");

  interrupts(); //enable interrupts (in case interrupts have been disabled)

  attachInterrupt(digitalPinToInterrupt(PIN2), toggle_ISR, RISING);  //trigger interrupt on seeing rising edge on PIN-2 
  Serial.print("SETUP DONE");

}


void loop()
{

  float accel_data = getAccelData(&myIMU);
  if(emergencyFilter(accel_data, prev)){
    circuitOperate(FLASHES);
  }
  prev = accel_data;  //update prev value for the next cycle
  delay(25);  //delay between consecutive data points is 25ms

}

