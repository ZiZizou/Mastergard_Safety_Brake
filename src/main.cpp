#include "SparkFunLSM6DSO.h"
#include "Wire.h"
#include <Arduino.h>

//#include "SPI.h"

LSM6DSO myIMU; //Default constructor is I2C, addr 0x6B
//Arduino UNO pins for I2C are A4 (SDA) and A5 (SCL)
float prev; //stores previous value of acceleration to be compared with current value from LSM6DSO

#define ACCEL_SCALE 16  //defining full scale accelerometer value to be 16g
#define CORRECTION 0.02   //defining accelerometer zero error
#define THRESHOLD 2250  //defining the threshold for determining whether brake was pushed in emergency
#define CHARGE_TIME_MS 250  //capacitor charging time in ms
#define DISCHARGE_TIME_MS 300 //capacitor discharge time in ms
#define FLASHES 4  //number of time led must flash before becoming solid

/*
@Function void IMUConnect()
parameters: None
does: initializes connection to the lsm6dso module and sets and returns full scale value
returns: None
*/
void IMUConnect() {

  Wire.begin();
  delay(10);
  bool ready = false;
  while(!ready){
    if( myIMU.begin() ){
      Serial.println("Ready.");
      ready = true;
    }
    else { 
      Serial.println("Could not connect to IMU.");
      Serial.println("Freezing");
    }
  }

  if( myIMU.initialize(BASIC_SETTINGS) )
    Serial.println("Loaded Settings.");

  myIMU.setAccelRange(ACCEL_SCALE);  //setting accelerometer full scale value to 16g
  Serial.print("Accelerometer full scale value - ");
  Serial.println(myIMU.getAccelRange());

  delay(500);

}

/*
@Function void getAccelData()
parameters: None
does: obtains acceleration value along one axis and applies correction to it
returns: double value of acceleration in x axis
*/
float getAccelData(){

  // Serial.print("\nAccelerometer:");
  // Serial.print(" X = ");
  float x_accel = myIMU.readFloatAccelX();
  x_accel -= CORRECTION;  //remove zero error from the value
  Serial.println(x_accel, 3);
  return x_accel;

}

/*
@Function void circuitOperate(int flash)
parameters: int flash - number of times light must flash
does: Uses PIN-7 and PIN-6 to drive relays to effectively flash an LED parallely
returns: NONE
*/
void circuitOperate(int flash){

  for(int i = 0; i< flash;i++){
    digitalWrite(PIN7, HIGH);
    delay(CHARGE_TIME_MS);  //light dimmed
    digitalWrite(PIN7, LOW);  
    delay(50); 
    digitalWrite(PIN6, HIGH);
    delay(DISCHARGE_TIME_MS); //preparing capacitor for charging again
    digitalWrite(PIN6, LOW);
  }

}

/*
@Function void emergencyFilter(float current_val, float prev_val)
parameters: current_val, prev_val - both floats
does: Uses parameter values and THRESH Macro to determine whether emergency braking condition is met
returns: boolean
*/
bool emergencyFilter(float current_val, float prev_val){

  if(current_val*prev_val<0){ //checks if the current val and the previous val have differing signs. 
  //If true, this means that deceleration is happening.
  float change = abs(current_val-prev_val);
  if(change>=THRESHOLD){
      return true;  //if there is sign change and change is larger than or equal to threshold then return true
   }
  }
  return false;
}

void setup() {

  Serial.begin(9600);
  pinMode(PIN7, OUTPUT);  //set relay1_switch pin to output
  pinMode(PIN6, OUTPUT);  //set relay2_switch pin to output 
  Serial.println("FLASHING PINS SET");

  delay(500); 
  
  IMUConnect();

}


void loop()
{

  float accel_data = getAccelData();
  if(emergencyFilter(accel_data, prev)){
    circuitOperate(FLASHES);
  }
  prev = accel_data;  //update prev value for the next cycle
  delay(25);  //delay between consecutive data points is 25ms

}
