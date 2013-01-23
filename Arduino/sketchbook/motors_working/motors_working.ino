
/*
*  This code is in the public domain.
*  (Do whatever you want with it.)
*/



// Need the Servo library
#include <Servo.h>

// This is our motor.
Servo myMotorL;
Servo myMotorR;

// This is the final output
// written to the motor.
String incomingString;


void setup(){
  Serial.begin(9600);
  myMotorL.attach(9);
  myMotorR.attach(10);
  Serial.println("initializing");
  
  Serial.println("Waiting");
}
void loop(){
  int i = 60;
  for (; i < 100; i++)
  {
      myMotorL.write(i);
      myMotorR.write(i-60);      
      Serial.println(i);
      delay(200);
  }
  for (; i > 59; i--)
  {
      myMotorL.write(i);
      myMotorR.write(i-60);      
      Serial.println(i);
      delay(200);
  }
  
  delay(10000);


  
  
  
  
  
/*  myMotor.write(75);
  delay(1000);
  myMotor.write(65);
    delay(10000);
    */
}

/*
// Set everything up
void setup()
{
  // Put the motor to Arduino pin #9
  Serial.begin(9600);
  myMotor.attach(9);
  Serial.println("initializing");
  myMotor.write(50);
  Serial.println("Waiting");
  while(!Serial.available()){}
  while(Serial.available())
  {
    int temp = Serial.read();
  }    
  Serial.println("Waiting for 2 sec");
  delay(2000);
  Serial.println("Waiting for 5 seconds");
  delay(5000);
  Serial.println("I am out");
}


void loop()
{
}
*/
