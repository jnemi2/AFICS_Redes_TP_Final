#include<EEPROM.h>

typedef struct data  // data to store in EEPROM
{
  float capacity;
} Data;

Data data;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
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
