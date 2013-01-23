#include "ITG3200.h"
#include "Arduino.h"
#include "Device_I2C.h"
#include "GlobalDefined.h"


float gyroRate[3] = {0.0,0.0,0.0};
//float gyroRateDegrees[3] = {0.0,0.0,0.0};
int   gyroZero[3] = {0,0,0};
long  gyroSample[3] = {0,0,0};
float gyroSmoothFactor = 1.0;
float gyroScaleFactor = 0.0;
float gyroHeading = 0.0;
unsigned long gyroLastMeasuredTime = 0;
byte gyroSampleCount = 0;

ITG3200::ITG3200(){}



void ITG3200::GyroscopeInit() {

  gyroScaleFactor = radians(1.0 / 14.375);  //  ITG3200 14.375 LSBs per °/sec
  updateRegisterI2C(ITG3200_ADDRESS, ITG3200_RESET_ADDRESS, ITG3200_RESET_VALUE); // send a reset to the device
  updateRegisterI2C(ITG3200_ADDRESS, ITG3200_LOW_PASS_FILTER_ADDR, ITG3200_LOW_PASS_FILTER_VALUE); // 10Hz low pass filter
  updateRegisterI2C(ITG3200_ADDRESS, ITG3200_RESET_ADDRESS, ITG3200_OSCILLATOR_VALUE); // use internal oscillator 
  calibrateGyro();
}


void ITG3200::measureGyro() {

  sendByteI2C(ITG3200_ADDRESS, ITG3200_MEMORY_ADDRESS);
  Wire.requestFrom(ITG3200_ADDRESS, ITG3200_BUFFER_SIZE);

  int gyroADC[3];
  measureSpecificGyroADC(gyroADC);

  for (byte axis = 0; axis <= ZAXIS; axis++) {
    gyroRate[axis] = filterSmooth(gyroADC[axis] * gyroScaleFactor, gyroRate[axis], gyroSmoothFactor);
  }
 
  // Measure gyro heading
  long int currentTime = micros();
  if (gyroRate[ZAXIS] > radians(1.0) || gyroRate[ZAXIS] < radians(-1.0)) {
    gyroHeading += gyroRate[ZAXIS] * ((currentTime - gyroLastMeasuredTime) / 1000000.0);
  }
  gyroLastMeasuredTime = currentTime;
}

void ITG3200::measureGyroSum() {

  sendByteI2C(ITG3200_ADDRESS, ITG3200_MEMORY_ADDRESS);
  Wire.requestFrom(ITG3200_ADDRESS, ITG3200_BUFFER_SIZE);
  
  measureSpecificGyroSum();
  
  gyroSampleCount++;
}

void ITG3200::evaluateGyroRate() {

  int gyroADC[3];
  evaluateSpecificGyroRate(gyroADC);
  gyroSample[XAXIS] = 0;
  gyroSample[YAXIS] = 0;
  gyroSample[ZAXIS] = 0;
  gyroSampleCount = 0;

  for (byte axis = 0; axis <= ZAXIS; axis++) {
    gyroRate[axis] = filterSmooth(gyroADC[axis] * gyroScaleFactor, gyroRate[axis], gyroSmoothFactor);
  }
  
  // Measure gyro heading
  long int currentTime = micros();
  if (gyroRate[ZAXIS] > radians(1.0) || gyroRate[ZAXIS] < radians(-1.0)) {
    gyroHeading += gyroRate[ZAXIS] * ((currentTime - gyroLastMeasuredTime) / 1000000.0);
  }
  gyroLastMeasuredTime = currentTime;
}

void ITG3200::measureSpecificGyroADC(int *gyroADC) {
  gyroADC[YAXIS] = readShortI2C() - gyroZero[YAXIS];
  gyroADC[XAXIS] = readShortI2C() - gyroZero[XAXIS];
  gyroADC[ZAXIS] = gyroZero[ZAXIS] - readShortI2C();
}

void ITG3200::measureSpecificGyroSum() {
  gyroSample[YAXIS] += readShortI2C();
  gyroSample[XAXIS] += readShortI2C();
  gyroSample[ZAXIS] += readShortI2C();
}

void ITG3200::evaluateSpecificGyroRate(int *gyroADC) {

  gyroADC[XAXIS] = (gyroSample[XAXIS] / gyroSampleCount) - gyroZero[XAXIS];
  gyroADC[YAXIS] = (gyroSample[YAXIS] / gyroSampleCount) - gyroZero[YAXIS];
  gyroADC[ZAXIS] = gyroZero[ZAXIS] - (gyroSample[ZAXIS] / gyroSampleCount);
}

void ITG3200::calibrateGyro() {

  int findZero[FINDZERO];
    for (byte calAxis = XAXIS; calAxis <= ZAXIS; calAxis++) {
      for (int i=0; i<FINDZERO; i++) {
        sendByteI2C(ITG3200_ADDRESS, (calAxis * 2) + ITG3200_MEMORY_ADDRESS);
        findZero[i] = readShortI2C(ITG3200_ADDRESS);
        delay(10);
      }
      if (calAxis == XAXIS) {
        gyroZero[YAXIS] = findMedianInt(findZero, FINDZERO);
      }
      else if (calAxis == YAXIS) {
        gyroZero[XAXIS] = findMedianInt(findZero, FINDZERO);
      }
      else {
        gyroZero[ZAXIS] = findMedianInt(findZero, FINDZERO);
      }
    }
}

void ITG3200::getGyroRate(float* gyroRateDegrees) {
	measureGyro();
	for (byte axis=0;axis<=ZAXIS;axis++) {
		gyroRateDegrees[axis] = gyroRate[axis]*180/PI;
	}
	//return gyroRateDegrees;
}
