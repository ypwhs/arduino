int hao=1;int zuodian=0,jidianqi=0;
void setup() {
  // put your setup code here, to run once:
  pinMode(2,INPUT);
  pinMode(9,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(5,OUTPUT);
  digitalWrite(6,HIGH);
  Serial.begin(115200);
}
long last=0;
void loop() {
  // put your main code here, to run repeatedly:
  
  int zhuangtai=digitalRead(2);
  digitalWrite(13,zhuangtai);
  digitalWrite(9,zhuangtai);
  digitalWrite(8,jidianqi);
  digitalWrite(5,jidianqi);
  digitalWrite(7,zuodian);
  long now=millis();
  char buf[2];
  if(Serial.available())
    {
      Serial.readBytes(buf,2);
      if(buf[0]=='0')jidianqi=0;
      else if(buf[0]=='1')jidianqi=1;
      if(buf[1]=='0')zuodian=0;
      else if(buf[1]=='1')zuodian=1;
    }
  if(now-last>1000){
  last=now;
  char a[20];
  sprintf(a,"%02dH%d",hao,zhuangtai);
  Serial.print(a);
  }
}
