#define PWMA 4
#define AIN2 5
#define AIN1 6
#define STBY 7
#define POWER 3

void setup() {
  // put your setup code here, to run once:
  pinMode(PWMA, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(STBY, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(POWER, OUTPUT);
  digitalWrite(POWER, HIGH);
  attachInterrupt(0, add, RISING);
  setDir(0);
  digitalWrite(STBY, HIGH);
  analogWrite(PWMA, 10);
  Serial.begin(115200);
}
long zhuanshu = 0;
void add(){
  zhuanshu++;
}
void setDir(bool d){
  if(d){
    digitalWrite(AIN2, LOW);
    digitalWrite(AIN1, HIGH);
    digitalWrite(13, HIGH);
  }else{
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    digitalWrite(13, LOW);
  }
}
long lastzhuanshu = 0;
int pwm = 20;
void loop() {
  // put your main code here, to run repeatedly:
  int spd = zhuanshu-lastzhuanshu;
  if(spd<10)pwm++;
  else pwm--;
  if(pwm<0|pwm>250)pwm=0;
  analogWrite(PWMA, pwm);
  Serial.print(spd);
  Serial.print("\t");
  Serial.println(pwm);
  lastzhuanshu = zhuanshu;
  delay(1);
}
