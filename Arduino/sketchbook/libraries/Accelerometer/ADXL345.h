#ifndef _AEROQUAD_ACCELEROMETER_H_
#define _AEROQUAD_ACCELEROMETER_H_

#include "Arduino.h"
#include "Device_I2C.h"
#include "GlobalDefined.h"

#define ADXL345_ADDRESS 		0x53  // ACCELEROMETER I2C ADDRESS
#define ADXL345_DEV_ID 			0x00
#define ADXL345_POWER_CTL 		0x2D
#define ADXL345_DATA_FORMAT 		0x31
#define ADXL345_DATA 			0x32
#define ADXL345_BW_RATE 		0x2C

#define SAMPLECOUNT 1000.0

class ADXL345
{
	public:
		ADXL345();
		void AccelerometerInit();
		void measureAccel();
		void measureAccelSum();
		void evaluateMetersPerSec();
		void computeAccelBias();
		void calculateAngles(int*);
		void getAccelAngles(int*);
		float* getMeterPerSecSec();

};

#endif


