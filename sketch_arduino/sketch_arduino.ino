#include<EEPROM.h>

#define TRIG 8
#define ECHO 9
#define VALVE_PIN A5

#define N_SAMPLES 5

typedef struct data  // data to store in EEPROM
{
  float depth;  // config
  float max_level;  // config
  bool autofill;  // config
  bool filling;
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

    Serial.println(msg);
    if (msg == "get levl"){  // msg = "get levl"
      Serial.print((((data.max_level + data.depth) - measure()) * 100 / data.depth));
      Serial.println("%");
    }
    else if (msg == "get stat") {  // msg = "get stat"
      Serial.println((data.filling));
    }
    else if (msg == "get atfl") {  // msg = "get atfl"
      Serial.println((data.autofill));
    }
    else if (msg == "get dept") {  // msg = "get dept"
      Serial.println((data.depth));
    }
    else if (msg == "get Mlvl") {  // msg = "get Mlvl"
      Serial.println((data.max_level));
    }
    else if (msg == "set atfl 1") {
      data.autofill = true;  // msg = "set atfl 1" : true | msg = "set atfl 0" : false
      EEPROM.put(0, data); 
    } else if (msg == "set atfl 0") {
      data.autofill = false;
      EEPROM.put(0, data);
    }
    else if (msg.substring(0,8) == "set dept") {  // msg = "set dept x"
      String val = msg.substring(8, msg.length() - 1);
      float newDepth = val.toFloat();
      if (newDepth != 0.0){
        data.depth = newDepth;
        EEPROM.put(0, data);
      }
      Serial.println(val);
      Serial.println(newDepth);
    }
    else if (msg.substring(0,8) == "set Mlvl") {  // msg = "set Mlvl x"
      String val = msg.substring(8, msg.length() - 1);
      float max_lvl = val.toFloat();
      if (max_lvl != 0.0){
        data.max_level = max_lvl;
        EEPROM.put(0, data);
      }
      Serial.println(max_lvl);
    }
    
  }

  if (data.autofill) {  
    float distance = measure();
    if (distance > data.max_level + data.depth / 2) {
      digitalWrite(VALVE_PIN, HIGH);
      data.filling = true;
     
    }
    else if (distance <= data.max_level) {
      digitalWrite(VALVE_PIN, LOW);
      data.filling = false;
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
