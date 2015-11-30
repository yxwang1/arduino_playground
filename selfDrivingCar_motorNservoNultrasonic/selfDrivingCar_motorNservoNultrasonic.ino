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
int enableA = 2;
int pinA1 = 1;
int pinA2 = 0;
 
int enableB = 7;
int pinB1 = 6;
int pinB2 = 5;

// servo param
int pinServo = 9;
Servo servo1;
int degree = 0;
int change = 1;

//Define Run variable
boolean run;
void setup() {
/* 
 pinMode(enableA, OUTPUT);
 pinMode(pinA1, OUTPUT);
 pinMode(pinA2, OUTPUT);
 
 pinMode(enableB, OUTPUT);
 pinMode(pinB1, OUTPUT);
 pinMode(pinB2, OUTPUT);
 
 run = true;
*/
 //Servo
 servo1.attach(pinServo); 
 Serial.begin(9600);

}
 
//command sequence
void loop() {
 
  delay(50);
  int distance = detectObstacleDistance();
  delay(50);
 /* if(run)
 {
 
 delay(2000);
 Serial.println("Enable motors");
 enableMotors();

 Serial.println("Forward 1000");
 forward(1000);
 coast(500);

 Serial.println("Backward 1000");
 backward(1500);
 coast(500);

 Serial.println("Forward 500");
 forward(500);
 brake(500);

 Serial.println("TurnLeft 500");
 turnLeft(500);
  Serial.println("TurnRight 500");
 turnRight(500);

 Serial.println("Disable motors");
 disableMotors();
 
 run = true;
 }
 */
}

// detect obstacle distance

int detectObstacleDistance() {
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

  return distance; 
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
 
void forward(int time)
{
 motorAForward();
 motorBForward();
 delay(time);
}
 
void backward(int time)
{
 motorABackward();
 motorBBackward();
 delay(time);
}
 
void turnLeft(int time)
{
 motorABackward();
 motorBForward();
 delay(time);
}
 
void turnRight(int time)
{
 motorAForward();
 motorBBackward();
 delay(time);
}
 
void coast(int time)
{
 motorACoast();
 motorBCoast();
 delay(time);
}
 
void brake(int time)
{
 motorABrake();
 motorBBrake();
 delay(time);
}
