void setup() {
  // put your setup code here, to run once:
  pinMode(A2,INPUT_PULLUP);
  Serial.begin(9600);
}
int x,y,z;
void read()
{
  x=analogRead(A0);
  y=analogRead(A1);
  z=digitalRead(A2);
  
  if(x<10)x=-1;
  else if(x>1000)x=1;
  else x=0;
  
  if(y<10)y=-1;
  else if(y>1000)y=1;
  else y=0;
}

int delaytime = 0;

void loop() {
  // put your main code here, to run repeatedly:
  read();
  
    if(x==-1)
    {
      Serial.write(0xAA);
      Serial.write(0x91);
      Serial.write(0xE7);
      Serial.write(0xFC);
      
      Serial.write(0xAA);
      Serial.write(0x91);
      Serial.write(0xD0);
      Serial.write(0xFC);
    }
    else if(x==1)
    {
      Serial.write(0xAA);
      Serial.write(0x91);
      Serial.write(0xE7);
      Serial.write(0xFC);
      
      Serial.write(0xAA);
      Serial.write(0x91);
      Serial.write(0xDF);
      Serial.write(0xFC);
      delay(delaytime);
    }
    
    if(y==-1)
    {
      Serial.write(0xAA);
      Serial.write(0x91);
      Serial.write(0xE2);
      Serial.write(0xFC);
      delay(delaytime);
    }
    else if(y==1)
    {
      Serial.write(0xAA);
      Serial.write(0x91);
      Serial.write(0xEC);
      Serial.write(0xFC);
      delay(delaytime);
    }
    
    if(z==0)
    {
      Serial.write(0xAA);
      Serial.write(0x91);
      Serial.write(0xE7);
      Serial.write(0xFC);
      delay(delaytime);
    }
    
}
