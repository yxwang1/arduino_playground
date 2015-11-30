/*
 The circuit for PIR:
 inputPin (middle): 5
 * 
 The circuit for accelerometer:
 analog 0: accelerometer self test
 analog 1: z-axis
 analog 2: y-axis
 analog 3: x-axis
 analog 4: ground
 analog 5: vcc
 */
#include <SoftwareSerial.h>
SoftwareSerial sigfox(3, 2); // ARDUINO RX, TX

// accelerometer params
// these constants describe the pins. They won't change:
const int groundpin = 18;             // analog input pin 4 -- ground
const int powerpin = 19;              // analog input pin 5 -- voltage
const int xpin = A3;                  // x-axis of the accelerometer
const int ypin = A2;                  // y-axis
const int zpin = A1;                  // z-axis (only on 3-axis models)

int THRESHOLD = 50;
int OCCURRENCE = 2;
int DURATION = 5000; // 5 secs

int preXVal = 0;
int preYVal = 0;
int preZVal = 0;

long preTimeMillis = 0;

int significantShakingCounter = 0;

// PIR params
const int ledPin = 13;                // choose the pin for the LED
const int pirInputPin = 5;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status
byte peopleCounter = 0;

// application control params
long lastHeartbeatTimeMillis = 0;

//sensorMask locations
int PRESSURE = 0;
int PANIC = 1;
int ACCEL = 2;
int TEMP = 3;
int PIR = 4;
int HEARTBEAT = 7;


void setup() {
  // SETUP OF THE SOFTWARESERIAL
  pinMode(3, INPUT);
  pinMode(2, OUTPUT);

  // pir related pins
  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(pirInputPin, INPUT);     // declare sensor as input

  // accelerometer realted pins
  // Provide ground and power by using the analog inputs as normal
  // digital pins.  This makes it possible to directly connect the
  // breakout board to the Arduino.  If you use the normal 5V and
  // GND pins on the Arduino, you can remove these lines.
  pinMode(groundpin, OUTPUT);
  pinMode(powerpin, OUTPUT);
  digitalWrite(groundpin, LOW);
  digitalWrite(powerpin, HIGH);

  sigfox.begin(9600);
  Serial.begin(9600);
}

void loop() {
  // data to send should be 12 bytes (or less - puts 0 on the MSB) hexadecimal transformed to a chain of character
  byte sensorMask = 0;
  long curTimeMillis = millis();
  boolean hasEarthquake = false;
  
  val = digitalRead(pirInputPin);  // read pir input value
  if (val == HIGH) {            // check if the input is HIGH
    digitalWrite(ledPin, HIGH);  // turn LED ON
    if (pirState == LOW) {
      // we have just turned on
      
      // We only want to print on the output change, not state
      pirState = HIGH;
      peopleCounter++;
      Serial.println(peopleCounter);
    }
  } else {
    digitalWrite(ledPin, LOW); // turn LED OFF
    if (pirState == HIGH){
      // we have just turned of
      Serial.println("Motion ended!");
      // We only want to print on the output change, not state
      pirState = LOW;
    }
  }

  if (hasSignificantShaking()) {
    significantShakingCounter++;
  }
 
  if (curTimeMillis - preTimeMillis >= DURATION) { 
    if (significantShakingCounter >= OCCURRENCE) {
      // has earthquake
      hasEarthquake = true;
    }
    significantShakingCounter = 0;
    preTimeMillis = curTimeMillis;
  }

  // message is either triggered by earthquake
  // or send every 5 mins for heartbeat
  if (hasEarthquake) {
    Serial.println("Sending earthquake message...");
    lastHeartbeatTimeMillis = curTimeMillis;
    SIGFOX_SEND(sensorMask, false, hasEarthquake, 0, peopleCounter);
    
  } else if ((curTimeMillis - lastHeartbeatTimeMillis) >= 60000) {
    //SEND DATA
    Serial.println("Sending heartbeat message..."); 
    lastHeartbeatTimeMillis = curTimeMillis;
    SIGFOX_SEND(sensorMask, true, hasEarthquake, 0, peopleCounter);
  }

  delay(100);
}

// earth quake is defined as having significant shaking for more than 2 times within 5 seconds
// a significant shaking is defined as the current reading - previous reading >= threshold for at lease one axis

boolean hasSignificantShaking() {
  boolean ret = false;
  int xVal = analogRead(xpin);
  int yVal = analogRead(ypin);
  int zVal = analogRead(zpin);

  if (preXVal == 0) {
    preXVal = xVal;
    preYVal = yVal;
    preZVal = zVal;
  }

  if (xVal - preXVal >= THRESHOLD || yVal - preYVal >= THRESHOLD || zVal - preZVal >= THRESHOLD) {
    Serial.println("shaking");
    ret = true;
  }
  preXVal = xVal;
  preYVal = yVal;
  preZVal = zVal;
  return ret;
}

String SIGFOX_SEND(byte sensorMask, boolean isHeartbeat, boolean hasEarthquake, byte temp, byte count){
  char* buf_str = (char*) malloc (10 + 1);
  String protocol="AT$SF=";
  sensorMask |= (1 << PIR);
  sensorMask |= (1 << ACCEL);
  if (isHeartbeat) {
    sensorMask |= (1 << HEARTBEAT);
  }
  sprintf(buf_str, "%02X\0", sensorMask);
  protocol += String(buf_str);
  // sensor state
  byte state = 0;
  if (hasEarthquake) {
    state |= (1 << ACCEL);
  }
  sprintf(buf_str, "%02X\0", state);
  protocol += String(buf_str); 
  // temperature sensor value
  sprintf(buf_str, "%02X\0", temp);
  protocol += String(buf_str); 
  // PIR count
  sprintf(buf_str, "%02X\0", count);
  protocol+= String(buf_str);
  protocol+="\r";
  sigfox.print(String(protocol));
  Serial.println(protocol);
  delay(1000);
  return "OK";
}
