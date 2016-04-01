#include <Servo.h>
int sensor = A0;
int led = A1;
Servo servo;
void setup() {
  servo.attach(9);
  servo.write(90);
  pinMode(sensor, INPUT);
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);
  Serial.begin(115200);
  Serial.println("Start");
}
int last = 0;
int s = 0;
int counter = 0;

//********************************
void count() {
  if (s % 10 == 0) {
    servo.write(120);
    delay(1000);
    servo.write(90);
  }
}
//********************************

void loop() {
  int value = analogRead(sensor);
  //  Serial.println(value);
  value = value > 512;
  if (value == 0)counter ++;
  else counter = 0;
  int now = counter > 50;
  if (now != last) {
    last = now;
    if (now) {
      s++;
      count();
      Serial.println(s);
    }
  }
  delay(1);
}
