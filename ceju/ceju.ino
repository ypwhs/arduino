const int TrigPin = 2; 
const int EchoPin = 3; 
float cm;
int id = 1;
void setup() 
{
Serial.begin(9600); 
pinMode(TrigPin, OUTPUT); 
pinMode(EchoPin, INPUT); 
}
void loop() 
{
  if(Serial.read()=='[')
  {
    char buffer[10] = {0};
    Serial.readBytesUntil(']',buffer, 10);
    int readid=0;
    sscanf(buffer,"%d",&readid);
    if(readid==id)
    {
      digitalWrite(TrigPin, LOW); //低高低电平发一个短时间脉冲去TrigPin 
      delayMicroseconds(2); 
      digitalWrite(TrigPin, HIGH); 
      delayMicroseconds(10); 
      digitalWrite(TrigPin, LOW); 
      
      cm = pulseIn(EchoPin, HIGH) / 58.0; //将回波时间换算成cm
      Serial.print(cm); 
      Serial.print("cm"); 
      Serial.println(); 
      delay(10); 
      }
  }

} 
