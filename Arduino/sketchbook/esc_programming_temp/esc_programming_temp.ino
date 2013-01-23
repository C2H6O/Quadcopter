#include "Servo.h"

#define MOTOR_PIN 9 // This is the left motor
#define MOTOR_PIN_R 10
#define MOTOR_MAX_SPEED 90 // NOT REALLY NEEDED
#define MOTOR_START_SPEED 60

/*
* This is the max/min speed values restriction
*/
int motor_left_minimum = 68;
int motor_left_maximum = 118;
int motor_right_minimum = 68;
int motor_right_maximum = 118;

int motor_current_speed = 0;
Servo motor; // This is the left motor
Servo motorRight;

String left;
String right;
void setup()
{

// 115200 default for blusmirf gold
//Serial.begin(115200);

// 9600 default for the xbee pro series1
Serial.begin(9600);

// Motor
delay(1000);
motor.attach(MOTOR_PIN); // This is the left motor
motorRight.attach(MOTOR_PIN_R);
motorStartAt(MOTOR_START_SPEED); // Arming procedure
delay(1500);
digitalWrite(9, LOW);
  delay(2000);
    Serial.println("Delay Done");
}



void loop()
{
  motor.write(50);

  
}





// Wrapper function for Servo's ".write(*speed*)" function
void motorSetSpeed(int speed)
{


// Don't let the motor go above or below pre-determined max and min
if (speed > MOTOR_MAX_SPEED)
speed = MOTOR_MAX_SPEED;
else if (speed < MOTOR_START_SPEED)
speed = MOTOR_START_SPEED;

motor.write(speed);
motorRight.write(speed);
motor_current_speed = speed;

//Serial.print("current motor speed = ");
//Serial.println(motor_current_speed);



}



void motorStartAt(int start_speed)
{
int i;
  for (i=0; i < start_speed; i++) {
motorSetSpeed(i);
Serial.println(i);
delay(100);
}
}
