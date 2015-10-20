void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  randomSeed(analogRead(A0));
}

float bat = 4.2;
void loop() {
  // put your main code here, to run repeatedly:
  if(random(10)==0)bat-=0.01;
  Serial.println(bat);
  delay(200);
}
