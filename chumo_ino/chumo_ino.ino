int touchpin = A0, light = 13, value = 0, sensitive = 20;
void setup() {
  // put your setup code here, to run once:
  pinMode(light, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  value = analogRead(touchpin);
  Serial.println(value);
  value=1023-value-sensitive;
  value/=2;
  if ( value > 0)
  {
    digitalWrite(light, HIGH);
    if(value<255)analogWrite(3, value);
    else digitalWrite(3,HIGH);
  }
  else
  {
    digitalWrite(light, LOW);
    digitalWrite(3,LOW);
  }
  delay(10);
}
