long last = 0;
long lastyouren = 0;

int fenshu = 0;

void logtime(){
  if(digitalRead(2)){
    last = millis();
  }else{
    int time = millis()-last;
    //Serial.println(time);
    if(time<1000){
      Serial.print("time=");
      Serial.println(time);
      if(fenshu<1000)fenshu+=200;
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("begin detect");
  attachInterrupt(0, logtime, CHANGE);
  pinMode(4, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);//power
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
}

int state = 0;
void loop() {
  // put your main code here, to run repeatedly:
  if(fenshu>0)fenshu--;
  else state=0;
  
  if(state==0){
    if(fenshu>600)state=1;
  }
  
  Serial.println(fenshu);
  delay(100);
  digitalWrite(11, digitalRead(2));
  digitalWrite(10, state);
  digitalWrite(4, !state);
  if(fenshu<1000)analogWrite(9, fenshu/4);
  else digitalWrite(9, HIGH);
}
