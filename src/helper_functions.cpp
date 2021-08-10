#include "helper_functions.h"

extern int charge_time;
extern int discharge_time;

/*
@Function void IMUConnect()
parameters: LSM6DSO* imu
does: initializes connection to the lsm6dso module and sets and returns full scale value
returns: None
*/
void IMUConnect(LSM6DSO* imu) {

  Wire.begin();
  delay(10);
  bool ready = false;
  while(!ready){
    if( imu->begin() ){
      Serial.println("Ready.");
      ready = true;
    }
    else { 
      Serial.println("Could not connect to IMU.");
      Serial.println("Freezing");
    }
  }

  if( imu->initialize(BASIC_SETTINGS) )
    Serial.println("Loaded Settings.");

  imu->setAccelRange(ACCEL_SCALE);  //setting accelerometer full scale value to 16g - max value recordable = 157 m/s^2
  Serial.print("Accelerometer full scale value - ");
  Serial.println(imu->getAccelRange());

  delay(500);

}

/*
@Function void getAccelData()
parameters: LSM6DSO* imu
does: obtains acceleration value along one axis and applies correction to it. Acceleration value returned is in g's. Example - 0.987 output indicates 0.987g
returns: double value of acceleration in x axis
*/
float getAccelData(LSM6DSO* imu){

  // Serial.print("\nAccelerometer:");
  // Serial.print(" X = ");
  float x_accel = imu->readFloatAccelX();
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
    delay(charge_time);  //light dimmed
    digitalWrite(PIN7, LOW);  
    delay(50); 
    digitalWrite(PIN6, HIGH);
    delay(discharge_time); //preparing capacitor for charging again
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