int DS=6,SHCP=5,RCK=4;
unsigned char t[10]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
void setup() {
  pinMode(6,OUTPUT);pinMode(5,OUTPUT);pinMode(4,OUTPUT);
  Serial.begin(115200);
}
void out()
{
  digitalWrite(RCK,0);
  digitalWrite(RCK,1);
}
void send(long a)
{
  int i;
  for(i=0;i<16;i++)
  {
    digitalWrite(SHCP,0);
    digitalWrite(DS,a%2);
    a=a>>1;
    digitalWrite(SHCP,1);
  }
}

long a=1;char f=1;
void loop() {
  send(a);
  out();
 a++;
if(f)a++;else a--;
if(a>0x8000)f=0;else if(a==0){f=1;a=1;}
  delay(100);
}
