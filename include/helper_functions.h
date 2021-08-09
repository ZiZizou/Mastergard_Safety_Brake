#ifndef HELPER_H
#define HELPER_H

#define ACCEL_SCALE 16  //defining full scale accelerometer value to be 16g
#define CORRECTION 0.02   //defining accelerometer zero error
#define THRESHOLD 1.750  //defining the threshold for determining whether brake was pushed in emergency
// #define CHARGE_TIME_MS 250  //capacitor charging time in ms
// #define DISCHARGE_TIME_MS 300 //capacitor discharge time in ms
#define FLASHES 4  //number of time led must flash before becoming solid

#include "SparkFunLSM6DSO.h"
#include <Arduino.h>

void IMUConnect(LSM6DSO* imu);
float getAccelData(LSM6DSO* imu);
void circuitOperate(int flash);
bool emergencyFilter(float current_val, float prev_val);

#endif