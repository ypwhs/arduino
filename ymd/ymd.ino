#define aDIO P2_7
#define aSCK P4_2
#define aRCK P4_1
#define LED1 P4_7
#define LED2 P4_7
#define LED3 P1_0
#define SW1 PUSH2
#define SW2 PUSH1


unsigned char t[10] = {
  0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};

int oneday = 1000;
void setup() {
  pinMode(aDIO, OUTPUT); 
  pinMode(aSCK, OUTPUT); 
  pinMode(aRCK, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(SW1, INPUT_PULLUP); 
  pinMode(SW2, INPUT_PULLUP);
  Serial.begin(115200);
}
void out()
{
  digitalWrite(aRCK, LOW);
  digitalWrite(aRCK, HIGH);
}

int i;
long last=0,now=0;
int lightPin=0;
long a=0,b=20000101;
int yue[]={0,31,28,31,30,31,30,31,31,30,31,30,31};
int y,m,d;
int button = 1;
void loop() {
  now = millis();
  if(now-last>oneday){
    Serial.println(b);
    last=now;
    b++;
    lightPin = LED1;
  }
  y = b/10000;
  m = b/100%100;
  d = b%100;
  if((y%4==0&&y%100!=0)|(y%400==0))yue[2]=29;
  else yue[2]=28;
  if(d>yue[m]){
    b+=100;
    b-=d-1;
    lightPin = LED2;
  }
  if(m>12){
    b+=10000;
    b-=1200;
    lightPin = LED3;
  }

  if(!digitalRead(SW1)){
    if(button){
      b+=100;
      lightPin = LED2;
    }
    button=0;
  }

  if(!digitalRead(SW2)){
    if(button){
      b+=10000;
      lightPin = LED3;
    }
    button=0;
  }

  if(lightPin){
    digitalWrite(lightPin, HIGH);
    delayMicroseconds(200);
    digitalWrite(lightPin, LOW);
    lightPin = 0;
  }

  if(digitalRead(SW1) && digitalRead(SW2))button=1;

  i=0;
  a=b;
  while(a){
    shiftOut(aDIO, aSCK, MSBFIRST, 1 << (7-i));
    if(i==0 | i==2 | i ==4)
      shiftOut(aDIO, aSCK, MSBFIRST, 0X80 + t[a%10]);
    else 
      shiftOut(aDIO, aSCK, MSBFIRST, t[a%10]);
    out();
    a/=10;
    i++;
  }
}

