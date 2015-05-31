#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;
float aax, aay, aaz, ggx, ggy, ggz; //存储量化后的数据
float Ax, Ay, Az; //单位 g(9.8m/s^2)
float Gx, Gy, Gz; //单位 °/s
float Angel_accX, Angel_accY, Angel_accZ; //存储加速度计算出的角度
long LastTime, NowTime, TimeSpan; //用来对角速度积分的

#define LED_PIN 13
bool blinkState = false;

int times = 0;

void setup() {
  Wire.begin();
  Serial.begin(115200);
  Serial.println("Serial connected");
  accelgyro.initialize();
  Serial.println(accelgyro.testConnection() ? "MPU6050 connected" : "MPU6050 failed");
  if (accelgyro.testConnection() == 0)setup();
  pinMode(LED_PIN, OUTPUT);
}
float xyz, xz, cosa;
int ci;
float GGx = 0, GGy = 0;
void loop() {
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  //======一下三行是对加速度进行量化，得出单位为g的加速度值
  Ax = ax / 16384.00;
  Ay = ay / 16384.00;
  Az = az / 16384.00;
  //==========以下三行是用加速度计算三个轴和水平面坐标系之间的夹角
  Angel_accX = atan(Ax / sqrt(Az * Az + Ay * Ay)) * 180 / 3.14;
  Angel_accY = atan(Ay / sqrt(Ax * Ax + Az * Az)) * 180 / 3.14;
  Angel_accZ = atan(Az / sqrt(Ax * Ax + Ay * Ay)) * 180 / 3.14;
  //==========以下三行是对角速度做量化==========
  ggx = gx / 131.00;
  ggy = gy / 131.00;
  ggz = gz / 131.00;
  //===============以下是对角度进行积分处理====== ==========
  NowTime = millis(); //获取当前程序运行的毫秒数
  TimeSpan = NowTime - LastTime; //积分时间这样算不是很严谨
  LastTime = NowTime;
  //下面这行是互补滤波,短期来看陀螺仪很准,所以比重是99.5%,但是陀螺仪有累积误差,所以需要加速度来互补,这里用减号是因为加速度算出来方向是反的
  Gx = (Gx + ggx * TimeSpan / 1000);
  GGx = (GGx + ggx * TimeSpan / 1000) * 0.99 + Angel_accZ * 0.01;
  
  GGy = (GGy + ggy * TimeSpan / 1000) * 0.99 + Angel_accY * 0.01;

  times++;
  if(!(times%20)){
    times = 0;
    Serial.print(Angel_accX); Serial.print("\t");
    Serial.print(Angel_accY); Serial.print("\t");
    Serial.print(Angel_accZ); Serial.print("\t");
    Serial.print(ggx); Serial.print("\t");
    Serial.print(ggy); Serial.print("\t");
    Serial.print(ggz); Serial.print("\t");
    Serial.print("Gx=");
    Serial.print(Gx); Serial.print("\t");
    Serial.print("GGx=");
    Serial.print(GGx); Serial.print("\t");
    Serial.print("GGy=");
    Serial.print(GGy); Serial.print("\t");
    Serial.println();
  }

}
