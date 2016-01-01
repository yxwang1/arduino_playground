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
int enableA = 6;
int pinA1 = 8;
int pinA2 = 7;
 
int enableB = 5;
int pinB1 = 4;
int pinB2 = 3;

// servo param
int pinServo = 9;
Servo servo1;
int degree = 0;
int change = 20;

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
 enableMotors(80);
}
 
//command sequence
void loop() {

 
  detectObstacleDistance();

  if (!backwardMode && distanceInFront > 20) {
    Serial.println("Forward ");
    forward();
  } else if (distanceOnLeft > 20) {
     Serial.println("TurnLeft");
    turnLeft();
    backwardMode = false;
  } else if (distanceOnRight > 20) {
     Serial.println("TurnRight");
    turnRight();
    backwardMode = false;
  } else {
     Serial.println("Backward");
    backward();
    //coast();
    backwardMode = true;
  }
  
  delay(100);
  coast();
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

  if (distance == 0) {
    distance = MAX_DISTANCE;
  }
  
  if (degree < 60 ) {
      distanceOnLeft = distance;
  } else if(degree < 120) {
      distanceInFront = distance;
  } else {
      distanceOnRight = distance;
  }

  String str = String("degree: ") + degree + String(" distanceOnLeft: ") + distanceOnLeft 
  + String(" distanceInFront: ") + distanceInFront 
  + String(" distanceOnRight: ") + distanceOnRight ;
  Serial.print((String)str); // Send ping, get distance in cm and print result (0 = outside set distance range)
  Serial.println("cm");
 
}
 
//Define Low Level H-Bridge Commands
 
//enable motors
void motorAOn(int s)
{
 analogWrite(enableA, s);
}
 
void motorBOn(int s)
{
 analogWrite(enableB, s);
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
 
void enableMotors(int s)
{
 motorAOn(s);
 motorBOn(s);
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
