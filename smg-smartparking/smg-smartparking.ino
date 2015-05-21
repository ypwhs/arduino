int aDIO = 6, aSCK = 5, aRCK = 4;
unsigned char t[11] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90,0xFF};
void setup() {
  pinMode(aDIO, OUTPUT); pinMode(aSCK, OUTPUT); pinMode(aRCK, OUTPUT);
  Serial.begin(115200);
  pinMode(7,OUTPUT);
  digitalWrite(7,HIGH);
}
void out()
{
  digitalWrite(aRCK, 0);
  digitalWrite(aRCK, 1);
}
int i; char a[8] = {0,0,0,0,10,10,10,0}, rec;
long last=0;
int n;int MAX=2;
int d[10]={};
void loop() {
  if(millis()-last>100)
  {last=millis();
      Serial.print("ASK[");Serial.print(n);Serial.print("]\n");
      char buffer[20]={0};
      if(Serial.available())
      {
        Serial.readBytesUntil(10,buffer, 20);
      int cm=-1,cm2=0,id=0;
      sscanf(buffer,"[%d]%d.%dcm",&id,&cm,&cm2);
      
      if(cm!=-1)
        {
        a[0]=(char)(cm/100);
        a[1]=(char)((cm/10)%10);
        a[2]=(char)(cm%10);
        a[3]=(char)(cm2/10);
        if(cm<7||(cm2<50&&cm==7))d[id]=1;else d[id]=0;
        for(i=1;i<=MAX;i++)if(d[i]==0)break;
        a[7]=i;
        }
      }
      n++;if(n>MAX)n=1;
     }
  for(i=7;i>=0;i--)  {
    shiftOut(aDIO, aSCK, MSBFIRST, 1 << i);
   // if(a[i])
      if(i==2)shiftOut(aDIO, aSCK, MSBFIRST, t[a[i]]-0x80);
      else shiftOut(aDIO, aSCK, MSBFIRST, t[a[i]]);
   // else shiftOut(aDIO, aSCK, MSBFIRST,0xFF);
    out();
  }
  out();
}
