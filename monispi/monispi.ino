int aDIO = 6, aSCK = 5, aRCK = 4;
void setup() {
  pinMode(aDIO, OUTPUT); pinMode(aSCK, OUTPUT); pinMode(aRCK, OUTPUT);
}
void out()
{
  digitalWrite(aRCK, 1);
  digitalWrite(aRCK, 0);
}
void loop() {
  shiftOut(aDIO, aSCK, MSBFIRST, 0x01);
  out();
}
