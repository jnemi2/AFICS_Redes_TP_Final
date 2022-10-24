#include<EEPROM.h>

#define TRIG 8
#define ECHO 9
#define VALVE_PIN A5

typedef struct data  // data to store in EEPROM
{
  float capacity;
} Data;

Data data;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(VALVE_PIN, OUTPUT);
  EEPROM.get(0, data);  // recovers the previous state
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    Serial.print("You sent me: ");
    Serial.println(data);
  }
}


float getDistance() {
  float echoTime;
  float distance = -1;
  
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  echoTime = pulseIn(ECHO, HIGH);

  distance = echoTime / 58;

  return distance;
}
