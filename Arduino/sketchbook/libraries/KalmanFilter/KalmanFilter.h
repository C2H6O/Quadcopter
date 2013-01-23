#ifndef KalmanFilter_h
#define KalmanFilter_h

#include "Arduino.h"
#include "MatrixMath.h"

class KalmanFilter
{
public:
	KalmanFilter();
	void KalmanFilterUpdateAngles(int*);
	void KalmanFilterInit(int*,float*);
};

#endif
