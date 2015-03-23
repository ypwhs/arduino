void setup() {
  // put your setup code here, to run once:
pinMode(13,OUTPUT);
}
int fade=1,i,f=1;
long last=0;
void loop() {
  // put your main code here, to run repeatedly:
for(i=0;i<250;i++)
{
  digitalWrite(13,i<fade);
  delayMicroseconds(1);
}
if(millis()-last>4){
  last=millis();
  fade+=f;
  if(fade==255||fade==0){f=-f;delay(100);}
}


}
