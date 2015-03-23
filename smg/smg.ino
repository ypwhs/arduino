int aDIO = 6, aSCK = 5, aRCK = 4;
unsigned char t[10] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};
void setup() {
  pinMode(aDIO, OUTPUT); pinMode(aSCK, OUTPUT); pinMode(aRCK, OUTPUT);
  Serial.begin(9600);
}
void out()
{
  digitalWrite(aRCK, 0);
  digitalWrite(aRCK, 1);
}
long i; char a[8] = { 0}, rec;
void loop() {
  rec = Serial.read();
  if (rec > 0) {
    for (i = 0; i < 7; i++)a[i] = a[i + 1];
    a[7] = rec - 48;
  }
  for(i=7;i>=0;i--)  {
    shiftOut(aDIO, aSCK, MSBFIRST, 1 << i);
    shiftOut(aDIO, aSCK, MSBFIRST, t[a[i]]);
    out();
  }
  out();
}
