void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
}
int i;
float s,o;
void loop() {
  // put your main code here, to run repeatedly:
s=analogRead(A0);
o=21.7*s/(1023-s);
Serial.print(s);
Serial.print("\t");
Serial.println(o);
delay(100);
}
