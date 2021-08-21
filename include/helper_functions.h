#ifndef HELPER_H
#define HELPER_H

#define ACCEL_SCALE 16  //defining full scale accelerometer value to be 16g
#define CORRECTION 0.02   //defining accelerometer zero error
#define DEFAULT_THRESHOLD 1.750  //defining the threshold for determining whether brake was pushed in emergency
#define FLASHES 4  //number of time led must flash before becoming solid
#define MIN_CHARGE_TIME 125
#define MIN_DISCHARGE_TIME 250
#define DYNAMIC_THRESHOLD_SLOPE ((DEFAULT_THRESHOLD*7)/11)  //slope for the linear equation used by dynamicThreshold function

#include "SparkFunLSM6DSO.h"
#include <Arduino.h>
#include <Wire.h>

void IMUConnect(LSM6DSO* imu);
float getAccelData(LSM6DSO* imu, uint8_t data);
void circuitOperate(int flash);
bool emergencyFilter(float current_val, float prev_val, float dynamicThreshold);
void generateRandomFlashTime();
void generateDynamicThreshold(float yAccelerometer, float *dynamicThreshold);

#endif