#include<EEPROM.h>

#define TRIG 8
#define ECHO 9
#define VALVE_PIN A5

#define N_SAMPLES 5

typedef struct data  // data to store in EEPROM
{
  float capacity;
  float depth;
  float max_level;
  bool autofill;
} Data;

Data data;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(VALVE_PIN, OUTPUT);
  EEPROM.get(0, data);  // recovers the previous state
  data.depth = 35;
  data.max_level = 3;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    String msg = Serial.readStringUntil('\n');  // Coms 
    /*Serial.print("You sent me: ");
    Serial.println(msg);*/

    if (msg == "nivel"){
      Serial.print(((data.max_level + data.depth) - measure()) * 100 / data.depth);
      Serial.println("%");
    }
  }

  if (data.autofill) {  
    float distance = measure();
    if (distance > data.max_level + data.depth / 2) {
      digitalWrite(VALVE_PIN, HIGH);
     
    }
    else if (distance <= data.max_level) {
      digitalWrite(VALVE_PIN, LOW);
    }
  }
}

float measure() {
  float distance = 0;
  for (int i = 0; i < N_SAMPLES; i++){
    distance += getDistance();
    delay(10);
  }
  distance = distance / N_SAMPLES;
  return distance;
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
