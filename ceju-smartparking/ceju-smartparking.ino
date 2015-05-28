int TrigPin = 2;
int EchoPin = 3;
float cm;
int id = 4;
void setup() 
{
Serial.begin(115200); 
pinMode(TrigPin, OUTPUT); 
pinMode(EchoPin, INPUT); 

pinMode(4,OUTPUT);pinMode(5,OUTPUT);pinMode(6,OUTPUT);
digitalWrite(4,HIGH);digitalWrite(5,LOW);
//给zigbee供电
}
int cishu=0;
void loop() 
{
  delay(10);
      digitalWrite(TrigPin, LOW); //低高低电平发一个短时间脉冲去TrigPin 
      delayMicroseconds(2); 
      digitalWrite(TrigPin, HIGH); 
      delayMicroseconds(10); 
      digitalWrite(TrigPin, LOW); 
      cm = pulseIn(EchoPin, HIGH , 11600) / 58.0; //将回波时间换算成cm
      if(cm==0)cm=200;
      if(cm<7)cishu++;else cishu=0;
      digitalWrite(6,cishu>5);
  if(Serial.read()=='[')
  {
    digitalWrite(13,1);delay(1);digitalWrite(13,0);
    char buffer[10] = {0};
    Serial.readBytesUntil(']',buffer, 10);
    int readid=0;
    sscanf(buffer,"%d",&readid);
    if(readid==id)
    {
      char bufferout[20]={0};
      sprintf(bufferout,"[%d]",id);
      Serial.print(bufferout);

      Serial.print(cm); 
      Serial.print("cm"); 
      Serial.println(); 
      }
  }
} 
