void setup() {
  // put your setup code here, to run once:
DDRB&=0xFF;
}
int a=0;
void loop() {
  // put your main code here, to run repeatedly:

if(a>=8)a=0;
PORTB=~a;
a++;
delay(100);
}
