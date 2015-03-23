#include <Servo.h> 
Servo myservo;
int offset = 45;
void setup() {
  // put your setup code here, to run once:
  pinMode(5,OUTPUT);pinMode(6,OUTPUT);pinMode(7,OUTPUT);
  digitalWrite(5,LOW);
  analogWrite(6,50);
  digitalWrite(7,LOW);
  myservo.attach(11);
  myservo.write(45);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  myservo.write(45);
  delay(1000);
  myservo.write(75);
  delay(1000);
}
