int sensitive = 100;
void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  Serial.begin(9600);
}
int a, i, s;
void loop() {
  // put your main code here, to run repeatedly:

  a = 1023 - analogRead(A0);
  if (a > sensitive) {
    digitalWrite(13, 1);
    delay(300);
  }
  else {
    s = 0;
    for (i = 0; i < 10; i++) {
      a = 1023 - analogRead(A0);
      Serial.print("--\t");
      Serial.println(a);
      s += (a > sensitive);
      delay(1);
    }
    if (s == 0)digitalWrite(13, 0);
  }
  Serial.println(a);
}
