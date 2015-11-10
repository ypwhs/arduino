#include <Servo.h>
Servo myservo;

int speed = 0;
int dir = 32;

void turn(int angle){
  myservo.write(angle+83);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  myservo.attach(9);
  turn(0);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available())
  {
    char data = Serial.read();
    Serial.print(data);
    int cmd = data & 0xF0;
    int val = data & 0x0F;

    if (cmd == 0xA0) {
      if (val == 1) { //w
        speed = 1;
      } else if(val == 0){  //release
        speed = 0;
      } else if (val == 2) { //s
        speed = -1;
      } else if (val == 3) { //a
        turn(dir);
      }  else if (val == 4) { //d
        turn(-dir);
      } else if (val == 5){
        turn(0);
      }
    } else if (cmd == 0xB0) {
      dir = val*4;
    }
    Serial.println();
  }

  if (speed == 0) {
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
  } else if(speed == -1){
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(6, HIGH);
    digitalWrite(7, LOW);
  }else if(speed == 1){
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
    digitalWrite(6, LOW);
    digitalWrite(7, HIGH);
  }
}
