/*
  AeroQuad v3.0 - March 2011
 www.AeroQuad.com
 Copyright (c) 2011 Ted Carancho.  All rights reserved.
 An Open Source Arduino based multicopter.
 
 This program is free software: you can redistribute it and/or modify 
 it under the terms of the GNU General Public License as published by 
 the Free Software Foundation, either version 3 of the License, or 
 (at your option) any later version. 
 
 This program is distributed in the hope that it will be useful, 
 but WITHOUT ANY WARRANTY; without even the implied warranty of 
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 GNU General Public License for more details. 
 
 You should have received a copy of the GNU General Public License 
 along with this program. If not, see <http://www.gnu.org/licenses/>. 
 */

#define ITG3200_ADDRESS			        0x68
#define ITG3200_MEMORY_ADDRESS			0x1D
#define ITG3200_BUFFER_SIZE			6
#define ITG3200_RESET_ADDRESS			0x3E
#define ITG3200_RESET_VALUE			0x80
#define ITG3200_LOW_PASS_FILTER_ADDR	        0x16
#define ITG3200_LOW_PASS_FILTER_VALUE	        0x1D	// 10Hz low pass filter
#define ITG3200_OSCILLATOR_ADDR			0x3E
#define ITG3200_OSCILLATOR_VALUE		0x01	// use X gyro oscillator
#define ITG3200_SCALE_TO_RADIANS		823.626831 // 14.375 LSBs per °/sec, / Pi / 180
#define FINDZERO 49
#define SAMPLECOUNT 400.0
#define ACCEL_ADDRESS 0x53

#include <Wire.h>
#include <Device_I2C.h>
#include <GlobalDefined.h>
#include <AQMath.h>

//ACCELEROMETER

float accelScaleFactor[3] = {0.038320312,-0.038320312,-0.038320312};
float runTimeAccelBias[3] = {0, 0, 0};
float accelOneG = 0.0;
float meterPerSecSec[3] = {0.0,0.0,0.0};
long accelSample[3] = {0,0,0};
byte accelSampleCount = 0;

//GYROSCOPE
float gyroRate[3] = {0.0,0.0,0.0};
int   gyroZero[3] = {0,0,0};
long  gyroSample[3] = {0,0,0};
float gyroSmoothFactor = 1.0;
float gyroScaleFactor = 0.0;
float gyroHeading = 0.0;
unsigned long gyroLastMesuredTime = 0;
byte gyroSampleCount = 0;
unsigned long timer;

float AxrAcc, AyrAcc, AzrAcc,Rvect;
int AxrAcc_int, AyrAcc_int, AzrAcc_int;

void setup() {

  Serial.begin(115200);
  //Serial.println("Accelerometer library test (ADXL345)");

  Wire.begin();

  initializeAccel();
  computeAccelBias();

  initializeGyro();
  calibrateGyro();
  timer = millis();
}

void loop() {

  if((millis() - timer) > 200) // 100Hz
  {
    timer = millis();
    measureAccel();
    measureGyro();
    
    	Serial.print(accelScaleFactor[0]);
	Serial.print("  ");
	Serial.print(accelScaleFactor[1]);
	Serial.print("  ");
	Serial.print(accelScaleFactor[2]);
	Serial.println();
    
    Rvect = sqrt(pow(meterPerSecSec[XAXIS],2) + pow(meterPerSecSec[YAXIS],2) + pow(meterPerSecSec[ZAXIS],2));
    /*Serial.print("RaccX: ");   
    Serial.print(meterPerSecSec[XAXIS]);
    Serial.print("\tRaccY: ");   
    Serial.print(meterPerSecSec[YAXIS]);
    Serial.print("\tRaccZ: ");   
    Serial.print(meterPerSecSec[ZAXIS]);
    Serial.print("\t|Racc|: ");   
    Serial.print(Rvect);
    Serial.println();
    */
    
  
    AxrAcc = atan2(sqrt(pow(meterPerSecSec[YAXIS],2) + pow(meterPerSecSec[ZAXIS],2)),meterPerSecSec[XAXIS]);
    AxrAcc *= 360/PI;
    AxrAcc_int = (int) AxrAcc;
    Serial.print(AxrAcc_int,DEC);
    Serial.print(",");    
    
    AyrAcc = atan2(sqrt(pow(meterPerSecSec[XAXIS],2) + pow(meterPerSecSec[ZAXIS],2)),meterPerSecSec[YAXIS]);
    AyrAcc *= 360/PI;
    AyrAcc_int = (int) AyrAcc;
    Serial.print(AyrAcc_int,DEC); 
    
    Serial.println();
    /*	
     Serial.print("\tx-G: ");
     Serial.print(meterPerSecSec[XAXIS]);
     //Serial.println();
     Serial.print("\ty-G: ");
     Serial.print(meterPerSecSec[YAXIS]);
     //Serial.println();
     Serial.print("\tz-G: ");
     Serial.print(meterPerSecSec[ZAXIS]);
     Serial.println();
     Serial.println();
     //Serial.println();
     
     Serial.print("\tRate YZ: ");
     Serial.print(degrees(gyroRate[XAXIS]));
     Serial.print("\tRate XZ: ");
     Serial.print(degrees(gyroRate[YAXIS]));
     Serial.print("\tRate XY: ");
     Serial.print(degrees(gyroRate[ZAXIS]));
     Serial.print("\tHeading: ");
     Serial.print(degrees(gyroHeading));
     Serial.println();
     Serial.println();
     Serial.println();
     */
  }
}

void measureSpecificGyroADC(int *gyroADC) {
  gyroADC[YAXIS] = readShortI2C() - gyroZero[YAXIS];
  gyroADC[XAXIS] = readShortI2C() - gyroZero[XAXIS];
  gyroADC[ZAXIS] = gyroZero[ZAXIS] - readShortI2C();
}

void calibrateGyro() {

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

void initializeGyro() {

  gyroScaleFactor = radians(1.0 / 14.375);  //  ITG3200 14.375 LSBs per °/sec
  updateRegisterI2C(ITG3200_ADDRESS, ITG3200_RESET_ADDRESS, ITG3200_RESET_VALUE); // send a reset to the device
  updateRegisterI2C(ITG3200_ADDRESS, ITG3200_LOW_PASS_FILTER_ADDR, ITG3200_LOW_PASS_FILTER_VALUE); // 10Hz low pass filter
  updateRegisterI2C(ITG3200_ADDRESS, ITG3200_RESET_ADDRESS, ITG3200_OSCILLATOR_VALUE); // use internal oscillator 
}

void measureGyro() {

  sendByteI2C(ITG3200_ADDRESS, ITG3200_MEMORY_ADDRESS);
  Wire.requestFrom(ITG3200_ADDRESS, ITG3200_BUFFER_SIZE);

  int gyroADC[3];
  measureSpecificGyroADC(gyroADC);

  for (byte axis = 0; axis <= ZAXIS; axis++) {
    gyroRate[axis] = filterSmooth(gyroADC[axis] * gyroScaleFactor, gyroRate[axis], gyroSmoothFactor);
  }
  invertGyroRate(gyroRate, XAXIS);
  // Measure gyro heading
  long int currentTime = micros();
  if (gyroRate[ZAXIS] > radians(1.0) || gyroRate[ZAXIS] < radians(-1.0)) {
    gyroHeading += gyroRate[ZAXIS] * ((currentTime - gyroLastMesuredTime) / 1000000.0);
  }
  gyroLastMesuredTime = currentTime;
}

void invertGyroRate(float * gyroRate, int index) {
  gyroRate[index] = gyroRate[index] * (-1);
}





// ACCELEROMETER

void initializeAccel() {

  updateRegisterI2C(ACCEL_ADDRESS, 0x2D, 1<<3);     // set device to *measure*
  updateRegisterI2C(ACCEL_ADDRESS, 0x31, 0x09);     // set full range and +/- 4G
  updateRegisterI2C(ACCEL_ADDRESS, 0x2C, 8+2+1);    // 200hz sampling
  delay(10); 
}
  
void measureAccel() {

  sendByteI2C(ACCEL_ADDRESS, 0x32);
  Wire.requestFrom(ACCEL_ADDRESS, 6);

  meterPerSecSec[YAXIS] = readReverseShortI2C() * accelScaleFactor[YAXIS] + runTimeAccelBias[YAXIS];
  
  meterPerSecSec[XAXIS] = readReverseShortI2C() * accelScaleFactor[XAXIS] + runTimeAccelBias[XAXIS];
  meterPerSecSec[ZAXIS] = readReverseShortI2C() * accelScaleFactor[ZAXIS] + runTimeAccelBias[ZAXIS];
}

void measureAccelSum() {

  sendByteI2C(ACCEL_ADDRESS, 0x32);
  Wire.requestFrom(ACCEL_ADDRESS, 6);
  
  accelSample[YAXIS] += readReverseShortI2C() ;
  accelSample[XAXIS] += readReverseShortI2C() ;
  accelSample[ZAXIS] += readReverseShortI2C() ;
  accelSampleCount++;
}

void evaluateMetersPerSec() {
	
  for (byte axis = XAXIS; axis <= ZAXIS; axis++) {
    meterPerSecSec[axis] = (accelSample[axis] / accelSampleCount) * accelScaleFactor[axis] + runTimeAccelBias[axis];
	accelSample[axis] = 0;
  }
  accelSampleCount = 0;		
}

void computeAccelBias() {
  
  for (int samples = 0; samples < SAMPLECOUNT; samples++) {
    measureAccelSum();
    delayMicroseconds(2500);
  }

  for (byte axis = 0; axis < 3; axis++) {
    meterPerSecSec[axis] = (float(accelSample[axis])/SAMPLECOUNT) * accelScaleFactor[axis];
    accelSample[axis] = 0;
  }
  accelSampleCount = 0;

  runTimeAccelBias[XAXIS] = -meterPerSecSec[XAXIS];
  runTimeAccelBias[YAXIS] = -meterPerSecSec[YAXIS];
  runTimeAccelBias[ZAXIS] = -9.8065 - meterPerSecSec[ZAXIS];
  //runTimeAccelBias[ZAXIS] = meterPerSecSec[ZAXIS];
  accelOneG = abs(meterPerSecSec[ZAXIS] + runTimeAccelBias[ZAXIS]);
}



