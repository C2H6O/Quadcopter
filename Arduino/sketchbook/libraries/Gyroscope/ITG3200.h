#ifndef _AEROQUAD_GYROSCOPE_ITG3200_COMMON_H_
#define _AEROQUAD_GYROSCOPE_ITG3200_COMMON_H_

#define ITG3200_ADDRESS 					0x68  // GYROSCOPE I2C ADDRESS
#define ITG3200_TEMP 						0x1B
#define ITG3200_DATA 						0x1D
#define ITG3200_SMPLRT_DIV 					0x15
#define ITG3200_DLPF 						0x16
#define ITG3200_PWR_MGM 					0x3E

#define ITG3200_IDENTITY 					0x69
#define ITG3200_MEMORY_ADDRESS				0x1D
#define ITG3200_BUFFER_SIZE					6
#define ITG3200_RESET_ADDRESS				0x3E
#define ITG3200_RESET_VALUE					0x80
#define ITG3200_LOW_PASS_FILTER_ADDR		0x16
#define ITG3200_LOW_PASS_FILTER_VALUE		0x1D	// 10Hz low pass filter
#define ITG3200_OSCILLATOR_ADDR				0x3E
#define ITG3200_OSCILLATOR_VALUE			0x01	// use X gyro oscillator
#define ITG3200_SCALE_TO_RADIANS			823.626831 // 14.375 LSBs per °/sec, / Pi / 180

#include "Arduino.h"
#include "GlobalDefined.h"
#include <Device_I2C.h>
#include <AQMath.h>

#define FINDZERO 49

class ITG3200 {
	public:
		ITG3200();
		void GyroscopeInit();
		void measureGyro();
		void calibrateGyro();
		void measureGyroSum();
		void evaluateGyroRate();
		
		void evaluateSpecificGyroRate(int *gyroADC);
		void measureSpecificGyroSum();
		void measureSpecificGyroADC(int *gyroADC);
		
		void getGyroRate(float*);
		
};


#endif
