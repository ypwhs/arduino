#include <Servo.h> 
Servo myservo;
void setup() 
{ 
  myservo.attach(9);
  randomSeed(analogRead(A0));
}
void loop() 
{
  myservo.write(random(180));
  delay(random(5000));
}
