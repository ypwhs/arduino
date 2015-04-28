void setup() {
  // put your setup code here, to run once:
  int i;
  for (i = 4; i < 8; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  Serial.begin(115200);
}
void loop() {
  // put your main code here, to run repeatedly:
  while (Serial.available()) {
    char a = Serial.read();
    a -= '1';
    if (a < 4) {
      digitalWrite(a + 3, !digitalRead(a + 3));
    } else if (a == 4) {
      int i;
      for (i = 4; i < 8; i++) {
        digitalWrite(i, LOW);
      }
    }
  }
}
