// Motor control based on Yu Hin Hau
// Robotic Car via H-Bridge (L298)

#include <Servo.h> // servo motor library
#include <NewPing.h> // ultrasonic sensor library

// ultrasonic params
#define ULTRASONIC_TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ULTRASONIC_ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
NewPing sonar(ULTRASONIC_TRIGGER_PIN, ULTRASONIC_ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

//infrared params
#define INFRARED_F1_OUT_PIN A1  //front 1
#define INFRARED_F2_OUT_PIN A2  //front 2

 
// Define Pins for motor drive
int enableA = 6;
int pinA1 = 8;
int pinA2 = 7;
 
int enableB = 5;
int pinB1 = 4;
int pinB2 = 3;

// servo parameters/variables
Servo servo1;
int SERVO_PIN = 9;
int degree = 0;
int change = 10;

//Define Run variable
int distanceOnRight = 50;
int distanceInFront = 50;
int distanceOnLeft = 50;

// variable to keep track of when the robot is going backward
boolean backwardMode = false;

// if distance on any direction is less than minDistance, robot will turn away
int minDistance = 30;

// motor speed
int forwardSpeed = 90;
//int turnSpeed = 40;

void setup() {
 // set pinMode for motor driver
 pinMode(enableA, OUTPUT);
 pinMode(pinA1, OUTPUT);
 pinMode(pinA2, OUTPUT);
 
 pinMode(enableB, OUTPUT);
 pinMode(pinB1, OUTPUT);
 pinMode(pinB2, OUTPUT);

 //Servo
 servo1.attach(SERVO_PIN);
  
 // setup baud rate for serial monitor connection
 Serial.begin(9600);

 Serial.println("Enable motors");
 enableMotors(forwardSpeed);
 
 // scan and wait to check surroundings
 scanDelay(1000);
}
 
//command sequence
// main control program
void loop() {
  // backwardmode is needed because if the robot is going backward, the distance in front will increase and preventing a physical loop
  if (!backwardMode && distanceInFront > minDistance) {
    Serial.println("Forward ");
    forward();
    scanDelay(50);
    coast();
  } else if (distanceOnRight > minDistance) {
     Serial.println("TurnRight");

     // brake to prevent robot from skidding when turning at high speed
     brake();
     scanDelay(50);
     turnRight();

     // getting rid of distance information from direction not in effect after turning right
     clearDistances(1);
     scanDelay(100);
     coast();
     backwardMode = false;
    
  } else if (distanceOnLeft > minDistance) {
     Serial.println("TurnLeft");

     // brake to prevent robot from skidding when turning at high speed
     brake();
     scanDelay(50);
     turnLeft();

     // getting rid of distance information from direction not in effect after turning left
     clearDistances(-1);
     scanDelay(100);
     coast();
     backwardMode = false;
    
  } else {
     Serial.println("Backward");

     // TODO: test and remove to find effects and determine if needed
     brake();
     backward();

     // getting rid of distance information from direction not in effect after going backwards
     clearDistances(0);
     scanDelay(100);
     coast();
     backwardMode = true;
   
  }
  

 //Serial.println("Disable motors");
 // disableMotors();
 
}

/**
 *  scan distance and wait in a loop until millisecond passes
 */
void scanDelay(long millisecond) {
  long i = 0;
  
  while (i < millisecond) {
    detectObstacleDistanceWithUltrasonic();
    detectObstacleDistanceWithInfrared();
    delay(50);
    i = i + 50;
  }
}

/**
 * Getting rid of distance information from direction not in effect after turning
 */
void clearDistances(int turnDirection) {
  if (turnDirection == -1) { // turn left
    distanceOnRight = distanceInFront;
    distanceInFront = distanceOnLeft;
    distanceOnLeft = MAX_DISTANCE;
  } else if (turnDirection == 1) { // turn right
    distanceOnLeft = distanceInFront;
    distanceInFront = distanceOnRight;
    distanceOnRight = MAX_DISTANCE;
  } else {
    distanceInFront = distanceInFront+2;
    distanceOnLeft = MAX_DISTANCE;
    distanceOnRight = MAX_DISTANCE;
  }
}
// detect obstacle distance

// using infrared to detect objects
void detectObstacleDistanceWithInfrared() {
  int infraredDistanceFront1 = analogRead(INFRARED_F1_OUT_PIN);
  int infraredDistanceFront2 = analogRead(INFRARED_F2_OUT_PIN);

  // if infrared sensors sense anything less than 100 out of 1100(infrared analog output is between 0 and 1100), it will change the distance in front to be 3, causing the robot to turn
  if (infraredDistanceFront1 < 100) {
    distanceInFront = 3;
  }
  if (infraredDistanceFront2 < 100) {
    distanceInFront = 3;
  }
}

// detecting with ultrasonic sensor
void detectObstacleDistanceWithUltrasonic() {

    // turning servo to carry ultrasonic from 0 to 180 and back
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

  // if ultrasonic doesn't recieve signal back, then it will ignore that certain reading
  if (distance == 0) {
    distance = MAX_DISTANCE;
    return;
  }
  
  // if ultrasonic senses something when the servo's degree is 60 or less, then it is considered to be on the right side
  if (degree < 40 ) {

    // save the smallest value on the right
    if (distance < distanceOnRight) {
      distanceOnRight = distance;
    }
    
  // if ultrasonic senses something when the servo's degree is between 70 and 140, then it is considered to be in front
  } else if(degree >= 70 && degree < 140) {
    
    // save the smallest value on the right
    if (distance < distanceInFront) {
      distanceInFront = distance;
    }
    
  // if ultrasonic senses something when the servo's degree is 160 or more, then it is considered to be on the left side
  } else if(degree > 160)  {
    
    // save the smallest value on the right
      if (distance < distanceOnLeft) {
        distanceOnLeft = distance;
      }
  } else {
    String str = String("degree: ") + degree + String(" ignored");
    Serial.println((String) str);
    return;
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
 motorAForward();
 motorBBackward();
}
 
void turnRight()
{
  motorABackward();
  motorBForward();
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
