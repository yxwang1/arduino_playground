// Motor control based on Yu Hin Hau
// Robotic Car via H-Bridge (L298)

#include <Servo.h>
#include <NewPing.h>

// ultrasonic params
#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

//See Low Level for Command Definitions
 
//Define Pins
int enableA = 10;
int pinA1 = 8;
int pinA2 = 7;
 
int enableB = 5;
int pinB1 = 4;
int pinB2 = 3;

// servo param
int pinServo = 9;
Servo servo1;
int degree = 0;
int change = 15;

//Define Run variable
int distanceOnRight = 50;
int distanceInFront = 50;
int distanceOnLeft = 50;
boolean backwardMode = false;

void setup() {
 
 pinMode(enableA, OUTPUT);
 pinMode(pinA1, OUTPUT);
 pinMode(pinA2, OUTPUT);
 
 pinMode(enableB, OUTPUT);
 pinMode(pinB1, OUTPUT);
 pinMode(pinB2, OUTPUT);
 


 //Servo
 servo1.attach(pinServo); 
 Serial.begin(9600);

 Serial.println("Enable motors");
 enableMotors();
}
 
//command sequence
void loop() {
 
  detectObstacleDistance();

  if (!backwardMode && distanceInFront > 20) {
    Serial.println("Forward ");
    forward();
    coast();
  } else if (distanceOnLeft > 20) {
     Serial.println("TurnLeft");
    turnLeft();
  } else if (distanceOnRight > 20) {
     Serial.println("TurnRight");
    turnRight();
  } else {
     Serial.println("Backward");
    backward();
    coast();
    backwardMode = true;
  }
  
  delay(200);
 //Serial.println("Disable motors");
 // disableMotors();
 
}

// detect obstacle distance

void detectObstacleDistance() {
    degree = degree + change;
  if (degree >= 180) {
    change = -change;
  
  }
  if (degree <= 0) {
    change = -change;

  }
  servo1.write(degree);  
  Serial.print("Ping: ");
 int distance = sonar.ping_cm();
  String str = String("degree: ") + degree + String(" distance: ") + distance;
  Serial.print((String)str); // Send ping, get distance in cm and print result (0 = outside set distance range)
  Serial.println("cm");

  
  if (degree < 61 ) {
    //if (distance != 0)
      distanceOnLeft = distance;
  } else if(degree < 121) {
    //if (distance != 0)
      distanceInFront = distance;
  } else {
    //if (distance != 0)
      distanceOnRight = distance;
  }

}
 
//Define Low Level H-Bridge Commands
 
//enable motors
void motorAOn()
{
 digitalWrite(enableA, HIGH);
}
 
void motorBOn()
{
 digitalWrite(enableB, HIGH);
}
 
 //disable motors
void motorAOff()
{
 digitalWrite(enableB, LOW);
}
 
void motorBOff()
{
 digitalWrite(enableA, LOW);
}
 
 //motor A controls
void motorAForward()
{
 digitalWrite(pinA1, HIGH);
 digitalWrite(pinA2, LOW);
}
 
void motorABackward()
{
 digitalWrite(pinA1, LOW);
 digitalWrite(pinA2, HIGH);
}
 
//motor B contorls
void motorBForward()
{
 digitalWrite(pinB1, HIGH);
 digitalWrite(pinB2, LOW);
}
 
void motorBBackward()
{
 digitalWrite(pinB1, LOW);
 digitalWrite(pinB2, HIGH);
}
 
//coasting and braking
void motorACoast()
{
 digitalWrite(pinA1, LOW);
 digitalWrite(pinA2, LOW);
}
 
void motorABrake()
{
 digitalWrite(pinA1, HIGH);
 digitalWrite(pinA2, HIGH);
}
 
void motorBCoast()
{
 digitalWrite(pinB1, LOW);
 digitalWrite(pinB2, LOW);
}
 
void motorBBrake()
{
 digitalWrite(pinB1, HIGH);
 digitalWrite(pinB2, HIGH);
}
 
//Define High Level Commands
 
void enableMotors()
{
 motorAOn();
 motorBOn();
}
 
void disableMotors()
{
 motorAOff();
 motorBOff();
}
 
void forward()
{
 motorAForward();
 motorBForward();

}
 
void backward()
{
 motorABackward();
 motorBBackward();
 
}
 
void turnLeft()
{
 motorABackward();
 motorBForward();
 
}
 
void turnRight()
{
 motorAForward();
 motorBBackward();
 
}
 
void coast()
{
 motorACoast();
 motorBCoast();
 
}
 
void brake()
{
 motorABrake();
 motorBBrake();
 
}
