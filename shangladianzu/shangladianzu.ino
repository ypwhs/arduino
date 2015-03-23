void setup() {
  // put your setup code here, to run once:
pinMode(2,INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(13,digitalRead(2));
}
