#define INFRARED_L_OUT_PIN A0  //left
#define INFRARED_F1_OUT_PIN A1  //front 1
#define INFRARED_F2_OUT_PIN A2  //front 2
#define INFRARED_R_OUT_PIN A3  //right

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
// detect obstacle distance
  int infraredDistanceLeft = analogRead(INFRARED_L_OUT_PIN);
  int infraredDistanceFront1 = analogRead(INFRARED_F1_OUT_PIN);
  int infraredDistanceFront2 = analogRead(INFRARED_F2_OUT_PIN);
  int infraredDistanceRight = analogRead(INFRARED_R_OUT_PIN);
Serial.println("front1");
Serial.println(infraredDistanceFront1);
Serial.println("front2");
Serial.println(infraredDistanceFront2);
Serial.println("left");
Serial.println(infraredDistanceLeft);
Serial.println("right");
Serial.println(infraredDistanceRight);
delay(1000);
}
