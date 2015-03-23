void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(0,OUTPUT);
}
int i;
char a[]="19950421";
void loop() {
  // put your main code here, to run repeatedly:
  for(i=0;i<strlen(a);i++)
  {
    Serial.print(a[i]);
    delay(200);
  }
  delay(1000);
}
