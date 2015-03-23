void setup() {
  // put your setup code here, to run once:
  pinMode(A0, INPUT_PULLUP);
  Serial.begin(9600);
}
int value, i, a;
void loop() {
  // put your main code here, to run repeatedly:
  value = analogRead(A0);
  Serial.println(value);
  digitalWrite(13, value < 1000);
  if (value < 1000)
  {
    if (a == 0)
    {
      for (i = 0; i < 255; i += 5) {
        analogWrite(3, i);
        delay(10);
      }
      a = 1;
    }
    else {
      for (i = 255; i > 0; i -= 5) {
        analogWrite(3, i);
        delay(10);
      }
      a = 0;
      digitalWrite(3,0);
    }
  }
  while (value < 1000) {
    delay(1);
    value = analogRead(A0);
    Serial.println(value);
  }
}
