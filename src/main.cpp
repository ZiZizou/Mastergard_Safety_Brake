#include "Wire.h"
#include <Arduino.h>
#include "helper_functions.h"
#include "toggle_interrupt.h"
#include <neotimer.h>

//#include "SPI.h"

extern int charge_time;
extern int discharge_time;
LSM6DSO myIMU; //Default constructor is I2C, addr 0x6B
//Arduino UNO pins for I2C are A4 (SDA) and A5 (SCL)
float prev; //stores previous value of acceleration to be compared with current value from LSM6DSO
float emergencyThreshold = DEFAULT_THRESHOLD;
Neotimer chargeTimer; //charge timer 
Neotimer dischargeTimer;  //discharge timer
bool flashing = false;
int flashes = 0;
bool dischargeTimer_state = false;
bool chargeTimer_state = false;




void setup() {

  Serial.begin(9600);
  pinMode(PIN7, OUTPUT);  //set relay1_switch pin to output
  pinMode(PIN6, OUTPUT);  //set relay2_switch pin to output 
  pinMode(PIN2, INPUT); //set pin-2 to be flashing mode selector 
  digitalWrite(PIN7, LOW);
  digitalWrite(PIN6, LOW);  //initially set both switching pins low
  Serial.println("FLASHING PINS SET");

  //EEPROM_setup(); //perform eeprom writes for different configurations if not already done

  //delay(500); 
  Serial.println("PRE IMU SETUP");
  IMUConnect(&myIMU);
  Serial.println("POST IMU SETUP");

  interrupts(); //enable interrupts (in case interrupts have been disabled)

  attachInterrupt(digitalPinToInterrupt(PIN2), toggle_ISR, RISING);  //trigger interrupt on seeing rising edge on PIN-2 
  Serial.print("SETUP DONE");
  prev = getAccelData(&myIMU, 1); //get first x axis acceleration data point and store it in the prev variable

}


void loop()
{

  float x_Accel_data = getAccelData(&myIMU, 1); //get acceleration data along the x axis
  float y_Accel_data = getAccelData(&myIMU, 2); //get acceleration data along the y axis
  if(emergencyFilter(x_Accel_data, prev, emergencyThreshold)&&(!flashing)){
    // circuitOperate(FLASHES);
    flashing = true;
    chargeTimer = Neotimer(charge_time);
    dischargeTimer = Neotimer(discharge_time);
  }
  
  //if switching between the emergency filter functions, uncomment the below 
  //code block and comment the above code block. 

  // if(emergencyFilterSimplified(x_Accel_data, prev, emergencyThreshold)&&(!flashing)){
  //   // circuitOperate(FLASHES);
  //   flashing = true;
  //   chargeTimer = Neotimer(charge_time);
  //   dischargeTimer = Neotimer(discharge_time);
  // }
  if (flashing&&(!dischargeTimer_state)&&(!chargeTimer_state)){
    // delay(100);
    digitalWrite(PIN7, HIGH); //toggle PIN7 high 
    Serial.println("charge timer start");
    chargeTimer.start();
    chargeTimer_state = true; //charge timer is counting
  }
  // generateDynamicThreshold(y_Accel_data, &emergencyThreshold);  //modify emergencyThreshold value based on acceleration along y axis
  if(chargeTimer.done()&&(!dischargeTimer_state)&&flashing){
    Serial.println("charge timer stop");
    chargeTimer_state = false;  //charge timer is done counting
    digitalWrite(PIN7, LOW); //toggle PIN7 low
    digitalWrite(PIN6, HIGH); //toggle PIN6 high
    dischargeTimer.start(); //start discharge timer
    Serial.println("discharge timer start");
    dischargeTimer_state = true;  //discharge
  }
  if(dischargeTimer.done()&&(!chargeTimer_state)&&flashing){
    dischargeTimer_state = false; //discharge timer is done counting
    Serial.println("discharge timer stop");
    digitalWrite(PIN6, LOW); //toggle PIN6 low
    flashes++;
    Serial.println(flashes);
  }
  if(flashes==FLASHES){
    flashing=false; //stop flashing
    flashes = 0;
  }
  prev = x_Accel_data;  //update prev value for the next cycle
  delay(25);  //delay between consecutive data points is 25ms

}

