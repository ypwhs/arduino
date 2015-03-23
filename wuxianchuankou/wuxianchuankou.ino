int H = 1;
void setup() {
  Serial.begin(57600);
  pinMode(12, INPUT_PULLUP);
  pinMode(2, OUTPUT);
}
int sen;
void loop()
{
  sen = digitalRead(12);
  if (!sen) {
    Serial.print(H);
    digitalWrite(2, HIGH); digitalWrite(13, HIGH);
    
    delay(30);
    digitalWrite(2, LOW); digitalWrite(13, LOW);
  }
  while (!sen) {
    sen = digitalRead(12);
    delay(1);
  }
}
