int button = 2, led = 13;
void setup() {
  // put your setup code here, to run once:
  pinMode(button, INPUT);
  pinMode(led, OUTPUT);
}
int a=1;
void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(button))
  {
      digitalWrite(led, a);
      a = 1 - a;
      while (digitalRead(button));
  }
}
