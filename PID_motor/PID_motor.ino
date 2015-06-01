#define PWMA 2
#define AIN2 3
#define AIN1 4
#define STBY 5

void setup() {
  // put your setup code here, to run once:
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(13, OUTPUT);
  setDir(0);
  digitalWrite(STBY, HIGH);
  analogWrite(PWMA, 20);
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
void loop() {
  // put your main code here, to run repeatedly:
  setDir(0);
}
