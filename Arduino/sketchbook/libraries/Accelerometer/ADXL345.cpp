#include "ADXL345.h"
#include "Arduino.h"
#include "Device_I2C.h"
#include "GlobalDefined.h"

float _accelScaleFactor[3] = {0.038320312,-0.038320312,-0.038320312};
float _runTimeAccelBias[3] = {0, 0, 0};
float _accelOneG = 0.0;
float _meterPerSecSec[3] = {0.0,0.0,0.0};
long _accelSample[3] = {0,0,0};
byte _accelSampleCount = 0;
//int _accelAngles[2] = {0,0};
//int _accelAngles[2] = {0,0};
float AxrAcc, AyrAcc, AzrAcc,Rvect;
int AxrAcc_int, AyrAcc_int, AzrAcc_int;

ADXL345::ADXL345(){}

void ADXL345::AccelerometerInit() {
  updateRegisterI2C(ADXL345_ADDRESS, 0x2D, 1<<3); 	// set device to *measure*
  updateRegisterI2C(ADXL345_ADDRESS, 0x31, 0x09);     // set full range and +/- 4G
  updateRegisterI2C(ADXL345_ADDRESS, 0x2C, 8+2+1);    // 200hz sampling
  delay(10);
  computeAccelBias(); 
}
  
void ADXL345::measureAccel() 
{
	sendByteI2C(ADXL345_ADDRESS, 0x32);
	Wire.requestFrom(ADXL345_ADDRESS, 6);
	for (byte axis = XAXIS; axis <= ZAXIS; axis++) {
		_meterPerSecSec[axis] = readReverseShortI2C() * _accelScaleFactor[axis] + _runTimeAccelBias[axis];
  }
}

void ADXL345::measureAccelSum() {

  sendByteI2C(ADXL345_ADDRESS, 0x32);
  Wire.requestFrom(ADXL345_ADDRESS, 6);
  for (byte axis = XAXIS; axis <= ZAXIS; axis++) {
    _accelSample[axis] += readReverseShortI2C();
  }
  _accelSampleCount++;
}

void ADXL345::evaluateMetersPerSec() {
	
  for (byte axis = XAXIS; axis <= ZAXIS; axis++) {
    _meterPerSecSec[axis] = (_accelSample[axis] / _accelSampleCount) * _accelScaleFactor[axis] + _runTimeAccelBias[axis];
	_accelSample[axis] = 0;
  }
  _accelSampleCount = 0;		
}

void ADXL345::computeAccelBias() {
  
  for (int samples = 0; samples < SAMPLECOUNT; samples++) {
    measureAccelSum();
    delayMicroseconds(2500);
  }

  for (byte axis = 0; axis < 3; axis++) {
    _meterPerSecSec[axis] = (float(_accelSample[axis])/SAMPLECOUNT) * _accelScaleFactor[axis];
    _accelSample[axis] = 0;
  }
  _accelSampleCount = 0;

  _runTimeAccelBias[XAXIS] = -_meterPerSecSec[XAXIS];
  _runTimeAccelBias[YAXIS] = -_meterPerSecSec[YAXIS];
  _runTimeAccelBias[ZAXIS] = -9.8065 - _meterPerSecSec[ZAXIS];

  _accelOneG = abs(_meterPerSecSec[ZAXIS] + _runTimeAccelBias[ZAXIS]);
}

void ADXL345::calculateAngles(int* _accelAngles) {
	
	//Serial.print("X: ");	
	//Serial.print(_meterPerSecSec[XAXIS],DEC);	
	//Serial.print("Y: ");
	//Serial.print(_meterPerSecSec[YAXIS],DEC);
	//Serial.print("Z: ");
	//Serial.print(_meterPerSecSec[ZAXIS],DEC);
	//Serial.println();
		
    Rvect = sqrt(pow(_meterPerSecSec[XAXIS],2) + pow(_meterPerSecSec[YAXIS],2) + pow(_meterPerSecSec[ZAXIS],2));
 
    AxrAcc = atan2(sqrt(pow(_meterPerSecSec[YAXIS],2) + pow(_meterPerSecSec[ZAXIS],2)),_meterPerSecSec[XAXIS]);
    
    AxrAcc = AxrAcc* 180/PI - 90;
    AxrAcc_int = (int) AxrAcc;
    _accelAngles[0] = AxrAcc_int;
    //Serial.print(AxrAcc_int,DEC);
    //Serial.print(",");    
    
    AyrAcc = atan2(sqrt(pow(_meterPerSecSec[XAXIS],2) + pow(_meterPerSecSec[ZAXIS],2)),_meterPerSecSec[YAXIS]);
    AyrAcc = AyrAcc* 180/PI -90;
    AyrAcc_int = (int) AyrAcc;
    _accelAngles[1] = AyrAcc_int;
    //Serial.print(AyrAcc_int,DEC); 
    //Serial.println();
}

void ADXL345::getAccelAngles(int* angles){
	measureAccel();
	calculateAngles(angles);
}

