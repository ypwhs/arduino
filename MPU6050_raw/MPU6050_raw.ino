#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"


MPU6050 accelgyro;

int16_t ax, ay, az;

#define LED_PIN 13
bool blinkState = false;

void setup() {
  Wire.begin();
  Serial.begin(115200);
  Serial.println("Serial connected");
  accelgyro.initialize();
  Serial.println(accelgyro.testConnection() ? "MPU6050 connected" : "MPU6050 failed");
  if(accelgyro.testConnection()==0)setup();
  pinMode(LED_PIN, OUTPUT);
}
float xyz, xz, cosa;
int ci;
void loop() {
  accelgyro.getAcceleration(&ax, &ay, &az);

  ax /= 200;
  ay /= 200;
  az /= 200;

  /*
  Serial.print(ax);Serial.print("\t");
  Serial.print(ay);Serial.print("\t");
  Serial.println(az);
  */

  xyz = ax * ax + ay * ay + az * az;
  xz = ax * ax + az * az;
  cosa = sqrt(xz / xyz);

  if (cosa > 0.866) {
    ci++;
    digitalWrite(LED_PIN, 1);
    if (ci <= 500)
    {
      Serial.print(cosa);
      delay(10);
      if (ci == 490) {
        Serial.println("ATD13691595989;");
        digitalWrite(2, 1);
        delay(5000);
        digitalWrite(2, 0);
      }
      if (ci % 25 == 0) {
        digitalWrite(2, 1);
        delay(30);
        digitalWrite(2, 0);
      }
    } else delay(1000);
  }
  else {
    ci = 0;
    digitalWrite(LED_PIN, 0);
  }

}
