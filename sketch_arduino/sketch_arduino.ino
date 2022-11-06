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
  pinMode(LED_BUILTIN, OUTPUT);
  EEPROM.get(0, data);  // recovers the previous state
  if (data.autofill)
    digitalWrite(LED_BUILTIN, HIGH);
  else
    digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    String msg = Serial.readStringUntil('\n');  // Coms 

    String param = msg.substring(4,8);
    if (msg.substring(0,3) == "get") {
      if (param == "levl"){  // msg = "get levl"
        Serial.print(min(100, max(((data.max_level + data.depth) - measure()) * 100 / data.depth, 0)));
        Serial.println("%");
      }
      else if (param == "stat") {  // msg = "get stat"
        Serial.println((data.filling));
      }
      else if (param == "atfl") {  // msg = "get atfl"
        Serial.println((data.autofill));
      }
      else if (param == "dept") {  // msg = "get dept"
        Serial.println((data.depth));
      }
      else if (param == "Mlvl") {  // msg = "get Mlvl"
        Serial.println((data.max_level));
      }
    }
    else if (msg.substring(0,3) == "set") {
      if (param == "atfl") {
        String val = msg.substring(9,10);  // msg = "set atfl 1" : true | msg = "set atfl 0" : false
        if (val == "1"){
          data.autofill = true;
          digitalWrite(LED_BUILTIN, HIGH);
        } else if (val == "0"){
          data.autofill = false;
          digitalWrite(LED_BUILTIN, LOW);
        }
        EEPROM.put(0, data);
      }
      else if (param == "dept") {  // msg = "set dept x"
        String val = msg.substring(9, msg.length());
        float newDepth = val.toFloat();
        if (newDepth != 0.0){
          data.depth = newDepth;
          EEPROM.put(0, data);
        }
      }
      else if (param == "Mlvl") {  // msg = "set Mlvl x"
        String val = msg.substring(9, msg.length() - 1);
        float max_lvl = val.toFloat();
        if (max_lvl != 0.0){
          data.max_level = max_lvl;
          EEPROM.put(0, data);
        }
      }
      
    }
    
  }

  if (data.autofill) {  
    float distance = getDistance();
    if (distance > data.max_level + data.depth / 2) {
      digitalWrite(VALVE_PIN, HIGH);
      if (!(data.filling)){
        data.filling = true;
        EEPROM.put(0, data);
      }
     
    }
    else if (distance <= data.max_level) {
      digitalWrite(VALVE_PIN, LOW);
      if (data.filling){
        data.filling = false;
        EEPROM.put(0, data);
      }
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
