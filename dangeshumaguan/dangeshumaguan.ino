int aDIO = 6, aSCK = 5, aRCK = 4;
unsigned char t[10] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};
void setup() {
  pinMode(aDIO, OUTPUT); pinMode(aSCK, OUTPUT); pinMode(aRCK, OUTPUT);
}
void out()
{
  digitalWrite(aRCK, 0);
  digitalWrite(aRCK, 1);
}
long a = 0, b, time, lasttime, i;
void loop() {
  a=a+1;
  shiftOut(aDIO, aSCK, MSBFIRST,~t[a % 10]);
  out();
  delay(500);
}
