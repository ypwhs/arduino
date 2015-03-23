void setup() {
Serial.begin(115200);
}
int a;
void loop() {
  if(millis()-a>100)
  {a=millis();
int s[4],i;
for(i=0;i<4;i++)
{s[i]=analogRead(2*i+A0);
Serial.print("\tS");
Serial.print(i);Serial.print("=");
Serial.print(s[i]);
//Serial.print("\tV=");
//Serial.print(s[i]*5/1023.0);
}
Serial.println("");
  }
}
