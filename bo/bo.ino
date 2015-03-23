int hz=1000;float a;
void setup() {
  // put your setup code here, to run once:
Serial.begin(57600);
a=1000000/hz;
}

void loop() {
  // put your main code here, to run repeatedly:
  PORTB=0XFF;
  delayMicroseconds(a);
  PORTB=0X00;
  delayMicroseconds(a);
}
