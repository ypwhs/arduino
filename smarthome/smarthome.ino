#include <dht.h>
dht DHT;
#define DHT11_PIN 8

#include <IRremote.h>
IRsend irsend;

void setup() {
  // put your setup code here, to run once:
  int i;
  for (i = 4; i < 8; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
  Serial.begin(115200);
}
void loop() {
  // put your main code here, to run repeatedly:
  int chk = DHT.read11(DHT11_PIN);
  if (Serial.available()) {
    char a = Serial.read();
    Serial.print(a);
    a -= '0';
    if (a < 4) {
      digitalWrite(a + 3, !digitalRead(a + 3));
    } else if (a == 4) {
      int i;
      for (i = 4; i < 7; i++) {
        digitalWrite(i, HIGH);
      }
    } else if (a == 5) {
      Serial.print("温度:");
      Serial.print(DHT.temperature, 1);
      Serial.print(",湿度:");
      Serial.println(DHT.humidity, 1);
    } else if (a == 6) {
      irsend.sendNEC(0xFFA25D, 32);
    }
  }
}
