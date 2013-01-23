//#define unsigned char byte
#include "AQMath.h"
#include "Arduino.h"
#include "GlobalDefined.h"
#include "ADXL345.h"
#include "ITG3200.h"
#include "Device_I2C.h"
#include "Wire.h"
#include "KalmanFilter.h"
#include "MatrixMath.h"
#include <avr/interrupt.h>  
#include <avr/io.h>

// Accelerometer
ADXL345  adxl345;
int   adxl345_angles[2];          
  //  adxl345_angles[0] = ROLL [degrees]
  //  adxl345_angles[1] = PITCH [degrees]

// Gyroscope
ITG3200 itg3200;
float   itg3200_gyroRate[3];
  //    itg3200_gyroRate[0] = ROLL [degrees/sec]
  //    itg3200_gyroRate[1] = PITCH [degrees/sec]
  //    itg3200_gyroRate[2] = YAW [degrees/sec]

//Kalman filter
KalmanFilter KF_1;
int KalmanFilterAngles[2];      
    // KalmanFilterAngles[0] = ROLL [degrees]
    // KalmanFilterAngles[1] = PITCH [degrees]

unsigned long timer;
unsigned long endtime;


void setup() {

  Serial.begin(115200);
  Wire.begin();

  adxl345.AccelerometerInit();
  itg3200.GyroscopeInit();
  KF_1.KalmanFilterInit(adxl345_angles, itg3200_gyroRate);
    Serial.println("Hello Setup");
    delay(2000);
  interrupt_init();
  Serial.print("wtf");
  timer = millis();
}

void loop() {
  timer = millis();
  Serial.println("Hello");
/*
  if((millis() - timer) > 10) // 100Hz
  {

    timer = millis();
    adxl345.getAccelAngles(adxl345_angles);
    itg3200.getGyroRate(itg3200_gyroRate);
    KF_1.KalmanFilterUpdateAngles(KalmanFilterAngles);
    endtime = millis();
    Serial.println(endtime-timer,DEC);
  }
*/
}

ISR(TIMER4_OVF_vect) {
  // Your code
  adxl345.getAccelAngles(adxl345_angles);
  itg3200.getGyroRate(itg3200_gyroRate);
  KF_1.KalmanFilterUpdateAngles(KalmanFilterAngles);
  endtime = millis() - timer;
  Serial.println(endtime);
  
  TCNT4  = 45535;       //Reset Timer Count to 45535 out of 65535
  TIFR4 = 0x00;          //Timer2 INT Flag Reg: Clear Timer Overflow Flag
  
};  


void interrupt_init() {
  //Setup Timer2 to generate interrupt every 10ms
  TCCR4B = 0x00;        //Disbale Timer3 while we set it up
  TCNT4  = 45535;       //Reset Timer Count to 45535 out of 65535
  TIFR4  = 0x00;        //Timer3 INT Flag Reg: Clear Timer Overflow Flag
  TIMSK4 = 0x01;        //Timer3 INT Reg: Timer3 Overflow Interrupt Enable
  TCCR4A = 0x00;        //Timer3 Control Reg A: Normal port operation, Wave Gen Mode normal
  TCCR4B = 0x02;        //Timer3 Control Reg B: Timer Prescaler set to 8
  interrupts();
  sei();
}
