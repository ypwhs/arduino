#include <Servo.h>
#include <SoftwareSerial.h>

Servo myservo;
//SoftwareSerial mySerial(9, 10);
int offset = 45;
int speed_max = 80;
// 
void setup() {
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);
  
  pinMode(5, OUTPUT); 
  pinMode(6, OUTPUT);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  
  myservo.attach(11);
  myservo.write(45);
  
  Serial.begin(115200);
  //mySerial.begin(115200);
  
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  attachInterrupt(1, jishu, FALLING);
}
long s=0;
void jishu() {
  if (PIND & 0X04) {
    s++;
  }
  else {
    s--;
  }
}

long lasttime = millis();
long last = 0;

void setdir(int indir) {
  myservo.write(offset + indir);
}
void setspeed(int inspd) {
  if (inspd > 0) {
    digitalWrite(5, LOW);
    analogWrite(6, inspd);
  } else if (inspd < 0) {
    digitalWrite(5, HIGH);
    analogWrite(6, inspd);
  } else {
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
  }
}

void loop() {
  long nowtime = millis();
  if (nowtime - lasttime > 10) {
    long delta = s - last;
    Serial.println(delta);
    last = s;
    lasttime = nowtime;
  }

  if (Serial.available())
  {
    char bufchar = Serial.read();
    if (bufchar == 'w') {
      setspeed(speed_max);
    }
    else if (bufchar == 's') {
      setdir(0);
      setspeed(-speed_max);
    }
    else if (bufchar == 'a') {
      setdir(-30);
    }
    else if (bufchar == 'd') {
      setdir(30);
    } else if (bufchar == 'x') {
      setspeed(0);
      setdir(0);
    }
  }
}


