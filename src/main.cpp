#include "Wire.h"
#include <Arduino.h>
#include "helper.h"

//#include "SPI.h"

LSM6DSO myIMU; //Default constructor is I2C, addr 0x6B
//Arduino UNO pins for I2C are A4 (SDA) and A5 (SCL)
float prev; //stores previous value of acceleration to be compared with current value from LSM6DSO



void setup() {

  Serial.begin(9600);
  pinMode(PIN7, OUTPUT);  //set relay1_switch pin to output
  pinMode(PIN6, OUTPUT);  //set relay2_switch pin to output 
  Serial.println("FLASHING PINS SET");

  delay(500); 
  
  IMUConnect(&myIMU);

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
