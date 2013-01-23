#include <TestLib.h>
#include "Arduino.h"
#include "Wire.h"
//#include "KalmanFilter.h"
#include <MatrixMath.h>

int adxl345_angles[2];
float itg3200_gyroRate[3];

int *testPtr;

TestLib test(1,2,3);
//KalmanFilter kf1(adxl345_angles,itg3200_gyroRate);

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  //testPtr = (int*) malloc(3*sizeof(int));

  adxl345_angles[0] = 1;
  adxl345_angles[1] = 2;
  //adxl345_angles[2] = 13;
  
  itg3200_gyroRate[0] = 3;
  itg3200_gyroRate[1] = 4;
  itg3200_gyroRate[2] = 5;
  test.ptrInitialize();
    testPtr = test.ptrFunc1(adxl345_angles);
    Serial.println("testPtr: ");
    Serial.println(*testPtr,DEC);
    Serial.println(*(testPtr+1),DEC);
    Serial.println(*(testPtr+2),DEC);
    Serial.println("adxl345_angles: ");
    Serial.println(adxl345_angles[0],DEC);
    Serial.println(adxl345_angles[1],DEC);
    Serial.println(adxl345_angles[2],DEC);
  //kf1.ptrInitialize();
  //kf1.printKalman();

}

void loop() {

  
  
  

  test.printPtr();
    *(testPtr) = *(testPtr) + 1;
  	Serial.print(*(testPtr + 0),DEC);
	Serial.println();
	Serial.print(*(testPtr + 1),DEC);
	Serial.println();
	Serial.print(*(testPtr + 2),DEC);
	Serial.println();


  
  /*
  
  Serial.print("MAIN PROGRAM: ");
  Serial.print(*(testPtr),DEC);
  Serial.println();
  
  Serial.print(*(testPtr+1),DEC);
  Serial.println();
  
  Serial.print(*(testPtr+2),DEC);
  Serial.println();
  */
  //kf1.printKalman();
  //adxl345_angles[0] = 22;
  //adxl345_angles[1] = 444;


 // kf1.KalmanAngles();
  
  
  //  test.print_a();
  
  //test.func1(1,2,3);
  //Serial.print("Hello");
  //test.func1(3,4,5);
  delay(5000);
  
  
}
