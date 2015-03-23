int CW1=7,PWM1=6,PWM2=5,CW2=4;
void setup() {
  // put your setup code here, to run once:
pinMode(CW1,OUTPUT);
pinMode(CW2,OUTPUT);
pinMode(PWM1,OUTPUT);
pinMode(PWM2,OUTPUT);
}
void set(int a)
{
  analogWrite(PWM1,abs(a));analogWrite(PWM2,abs(a));
  if (a>0)
  {digitalWrite(CW1,HIGH);digitalWrite(CW2,HIGH);}
  else if(a<0){digitalWrite(CW1,LOW);digitalWrite(CW2,LOW);}
  else {digitalWrite(PWM1,LOW);digitalWrite(PWM2,LOW);}
}
void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(CW1,LOW);
digitalWrite(CW2,HIGH);
digitalWrite(PWM1,HIGH);
digitalWrite(PWM2,HIGH);
}
