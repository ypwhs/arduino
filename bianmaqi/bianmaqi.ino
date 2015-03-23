void setup() {
  // put your setup code here, to run once:
pinMode(2,INPUT_PULLUP);
pinMode(3,INPUT_PULLUP);
Serial.begin(115200);
attachInterrupt(1, fangxiang, FALLING);
}
long s=0,last=0;
void fangxiang(){
  if(PIND&0X04){
    s++;
  }
  else {
  s--;
  }
}
int speed=0,x=0;
void loop() {
speed=(s-last);
last=s;
Serial.print(s);
Serial.print("\t");
Serial.println(speed);
if(speed>0)digitalWrite(13,1);else digitalWrite(13,0);
delay(10);
}
