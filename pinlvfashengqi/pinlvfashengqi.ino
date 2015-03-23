void setup() {
  // put your setup code here, to run once:
  pinMode(2,OUTPUT);
  Serial.begin(115200);
}

long f=100,delaytime=1000000,mil=1000000;int p2=0;char buf[20]={0};
void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available())
  {
    Serial.readBytesUntil(10,buf,18);
    sscanf(buf,"%ld",&f);
    delaytime=mil/f;
    Serial.print("f=");
    Serial.print(f);
    Serial.print("Hz\tdelaytime=");
    Serial.print(delaytime);
    Serial.println("us");
  }
  if(delaytime<10000)delayMicroseconds(delaytime);
  else delay(delaytime/1000);
  digitalWrite(2,p2);
  p2=1-p2;
}
