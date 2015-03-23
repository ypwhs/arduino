#include <Servo.h>
Servo s1,s2,s3,s4;
int offset[]={0,0,0,0};
void setup() {
  digitalWrite(13,HIGH);
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  s1.attach(3);s2.attach(5);s3.attach(6);s4.attach(9);
  s1.write(90+offset[0]);
  s2.write(90+offset[1]);
  s3.write(90+offset[2]);
  s4.write(90+offset[3]);
  pinMode(10,INPUT_PULLUP);
  pinMode(12,OUTPUT);
  digitalWrite(12,LOW);
  digitalWrite(13,LOW);
  while(digitalRead(10));
  digitalWrite(13,HIGH);
//  while(!Serial.available());
  
}
void loop() {
  // put your main code here, to run repeatedly:
  int i;
  float theta=30;
  float dly=40;

//  stop();
//  delay(dly*10);
  
  s1.write(90+theta);
  s3.write(90-theta);
  delay(dly*10);
  stop();
  delay(dly*10);
  s2.write(90-theta);
  s4.write(90+theta);
  delay(dly*10);
  stop();
  delay(dly*10);
}

void stop()
{
  s1.write(90);s2.write(90);s3.write(90);s4.write(90);
}
