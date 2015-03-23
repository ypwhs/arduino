int led = 0;
int pinr = 9, ping = 10, pinb = 11;
int c[3] = {0, 0, 0};
int a[3] = {0, 0, 0};
void setup() {
  // put your setup code here, to run once:
  pinMode(9, OUTPUT); pinMode(10, OUTPUT); pinMode(11, OUTPUT);
  Serial.begin(9600);
}
void bianse(int * c1)
{
  int l, i;
  for (i = 0; i < 3; i++) {
    if (c1[i] != c[i]) {
      l = (c1[i] > c[i]) * 2 - 1;
      while (c1[i] != c[i]) {
        c[i] += l;
        analogWrite(i + 9, c[i]);
        delay(5);
      }
      a[ millis() % 4 - 1] = millis() % 256;
      delay(millis() / 100 % 500);
      a[ millis() % 4 - 1] = millis() % 256;
    }
  }
  Serial.print("c:");
  Serial.print(c[0]);
  Serial.print(",");
  Serial.print(c[1]);
  Serial.print(",");
  Serial.println(c[2]);

}
void loop() {
  // put your main code here, to run repeatedly:
  
  a[ millis() % 4 - 1] = millis() % 256;
  bianse(a);
}
