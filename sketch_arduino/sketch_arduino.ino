bool led_builtin_on = false;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}
void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    led_builtin_on = true;
    digitalWrite(LED_BUILTIN, led_builtin_on);
    String data = Serial.readStringUntil('\n');
    Serial.print("You sent me: ");
    Serial.println(data);
  }
  else {
    led_builtin_on = (led_builtin_on) ? false : true;
    digitalWrite(LED_BUILTIN, led_builtin_on);
    delay(500);
  }
}
