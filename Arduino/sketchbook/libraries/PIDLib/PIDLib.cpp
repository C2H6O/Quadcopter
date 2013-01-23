#include "Arduino.h"

unsigned long lastTime;
float Input, Output, Setpoint;
float ITerm, lastInput;
float kp,ki,kd;
int SampleTime = 1000;
float outMin, outMax;
bool inAuto = false;

#define MANUAL 0
#define AUTOMATIC 1


PIDLib::PIDLib() 
{

}

void PIDLib::Compute()
{
	if (!inAuto) return;
	//How long since we last calculated?
	unsigned long now = millis();
	unsigned long timeChange = now - lastTime;
	if (timeChange > SampleTime) 
	{
		//Compute all the working error variables
		float error = Setpoint - Input;
		ITerm = ki * error;
		if (ITerm > outMax) ITerm = outMax;
		else if (ITerm < outMin) ITerm = outMin;	
		double dInput = Input - lastInput;

		//Compute PID output
		Output = (kp * error) + (ITerm) - (kd * dInput);
		if (Output > outMax) Output = outMax;
		else if (Output < outMin) Output = outMin;

		//Remember some variables for next time
		lastInput= Input;
		lastTime = now;
	}
}

void PIDLib::PIDTuning(float Kp, float Ki, float Kd)
{
	float SampleTimeInSec = ((float)SampleTime)/1000;
	kp = Kp;
	ki = Ki * SampleTimeInSec;
	kd = Kd / SampleTimeInSec;
}

void PIDLib::PIDSetSampleTime(int NewSampleTime)
{
	if (NewSampleTime > 0)
	{
		float ratio = (float) NewSampleTime / (float) SampleTime;
		ki *= ratio;
		kd /= ratio;
		SampleTime = (unsigned long) NewSampleTime;
	}

}

void PIDLib::PIDSetOututLimits(float Min, float Max)
{
	if (Min > Max) return;
	outMin = Min;
	outMax = Max;
	if (Output > outMax) Output = outMax;
	else if (Output < outMin) Output = outMin;

	if (ITerm > outMax) ITerm = outMax;
	else if (ITerm < outMin) ITerm = outMin;
}

void PIDLib::PIDSetMode(int Mode)
{
	inAuto = (Mode == AUTOMATIC);
}
