#include <Servo.h> 
Servo myservo;
void setup() 
{ 
  myservo.attach(9);
}
  int j=120,f=20,de=800;
void loop() 
{ 
  myservo.write(j+analogRead(A0)%f);
  delay(de/2+analogRead(A0)%(de/2));
  myservo.write(j-analogRead(A0)%f);
  delay(de/2+analogRead(A0)%(de/2));
} 
