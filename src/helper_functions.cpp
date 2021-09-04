#include "helper_functions.h"
#include <math.h>
// #include <neotimer.h>


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
parameters: LSM6DSO* imu, uint8_t selection
does: obtains acceleration value along axis specified by selection parameter and applies correction to it. Acceleration value returned is in g's. Example - 0.987 output indicates 0.987g
if selection is 1, then return value is accln along x axis. If 2, then accln along y axis. If 3 then accln along z axis.
returns: double value of acceleration in the axis specified by the selection parameter.
*/
float getAccelData(LSM6DSO* imu, uint8_t selection){

  // Serial.print("\nAccelerometer:");
  // Serial.print(" X = ");
  float returnData = 0; //default value if no correct value of selection is used
  if(selection==1){
    returnData = imu->readFloatAccelX();
  }
  else if(selection==2){
    returnData = imu->readFloatAccelY();
  }
  else if(selection==3){
    returnData = imu->readFloatAccelZ();
  }
  returnData -= CORRECTION;  //remove zero error from the value
  // Serial.println(returnData, 3);
  return returnData;

}

// /*
// @Function void circuitOperate(int flash)  ***REDUNDANT FUNCTION
// parameters: int flash - number of times light must flash
// does: Uses PIN-7 and PIN-6 to drive relays to effectively flash an LED parallely
// returns: NONE
// */
// void circuitOperate(int flash){

//   delay(100); //initial delay for brake light to switch on
//   for(int i = 0; i< flash;i++){
//     digitalWrite(PIN7, HIGH);
//     delay(charge_time);  //light dimmed
//     digitalWrite(PIN7, LOW);  
//     delay(50); 
//     digitalWrite(PIN6, HIGH);
//     delay(discharge_time); //preparing capacitor for charging again
//     digitalWrite(PIN6, LOW);
//   }

// }

/*
@Function void emergencyFilter(float current_val, float prev_val)
parameters: float current_val, float prev_val, float dynamicThreshold
does: Uses parameter values to determine whether emergency braking condition is met
returns: boolean
*/
bool emergencyFilter(float current_val, float prev_val, float dynamicThreshold){

  if(current_val*prev_val<0){ //checks if the current val and the previous val have differing signs. 
  //If true, this means that deceleration is happening.
  float change = abs(current_val-prev_val);
  if(change>=dynamicThreshold){
      return true;  //if there is sign change and change is larger than or equal to threshold then return true
   }
  }
  return false;
}

//if switching filter function, comment the above function and uncomment the 
//below function

// /*
// @Function void emergencyFilterSimplified(float current_val, float prev_val)
// parameters: float current_val, float prev_val, float dynamicThreshold
// does: Uses parameter values to determine whether emergency braking condition is met.
//       Does not check for sign change in acceleration value and simply operates on whether the 
//       g-force threshold is met or exceeded.
// returns: boolean
// */
// bool emergencyFilterSimplified(float current_val, float prev_val, float dynamicThreshold){
  

//   float change = abs(current_val-prev_val);
//   if(change>=dynamicThreshold){
//       return true;  //if there is sign change and change is larger than or equal to threshold then return true
//    }
//   return false;
// }

/*
@Function void generateRandomFlashTime()
parameters: none
does: Uses random module to generate charge and discharge times and stores them in charge_time and discharge_time variables
returns: None
*/
void generateRandomFlashTime(){

  charge_time = random(MIN_CHARGE_TIME, 800);
  discharge_time = random(MIN_DISCHARGE_TIME,800);

}


/*
@Function void generateDynamicThreshold()
parameters: float yAccelerometer, float *dynamicThreshold
does: Uses yAccelerometer value to judge inclination and dynamically change the Emergency Braking Threshold via its pointer
returns: None
*/
void generateDynamicThreshold(float yAccelerometer, float *dynamicThreshold){

  float inclinationAngle = acos(yAccelerometer);
  *dynamicThreshold = inclinationAngle*DYNAMIC_THRESHOLD_SLOPE + DEFAULT_THRESHOLD;

}
