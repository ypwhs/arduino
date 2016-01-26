#include <Servo.h>

Servo servo;
int value = 30;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  servo.attach(9);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  ting();
  servo.write(value);
}

void qian(){
  digitalWrite(3, HIGH);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, HIGH);
}

void zuo(){
  digitalWrite(3, HIGH);
  digitalWrite(5, LOW);
  digitalWrite(6, HIGH);
  digitalWrite(7, LOW);
}

void you(){
  digitalWrite(3, LOW);
  digitalWrite(5, HIGH);
  digitalWrite(6, LOW);
  digitalWrite(7, HIGH);
}

void you2(){
  analogWrite(3, 190);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, HIGH);
}

void hou(){
  digitalWrite(3, LOW);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, LOW);
}

void ting(){
  digitalWrite(3, LOW);
  digitalWrite(5, LOW);
  digitalWrite(7, LOW);
  digitalWrite(6, LOW);
}


void xx(){
  if(value<180)value+=10;
  servo.write(value);
}

void zz(){
  if(value>0)value-=10;
  servo.write(value);
}

void serialEvent() {
  char c = (char)Serial.read();
  if(c=='w')qian();
  else if(c=='a')zuo();
  else if(c=='s')hou();
  else if(c=='d')you();
  else if(c=='f')you2();
  else if(c=='x')xx();
  else if(c=='z')zz();
  else if(c=='t')ting();
}

void loop() {
  // put your main code here, to run repeatedly:
}
