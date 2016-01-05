#include "MPU6050.h"
#include "Wire.h"
MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;
float Ax, Ay, Az; //单位 g(9.8m/s^2)
float Gx=0, Gy=0, Gz=0; //单位 °/s
float aax, aay, aaz, ggx, ggy, ggz; //存储量化后的数据
float Angel_accX, Angel_accY, Angel_accZ; //存储加速度计算出的角度
long LastTime, NowTime, TimeSpan; //用来对角速度积分的


void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(115200);
  // initialize device
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();

  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

}

void loop() {
  // put your main code here, to run repeatedly:
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

  Gx = (Gx - ggy*TimeSpan/1000) * 0.99 + 0.01 * Angel_accX;
  Gy = (Gy + ggx*TimeSpan/1000) * 0.99 + 0.01 * Angel_accY;
  
  
  Serial.print(Angel_accX);
  Serial.print("\t");
  Serial.print(Angel_accY);
  Serial.print("\t");
  Serial.print(Angel_accZ);
  Serial.print("\t");
  Serial.print(ggx);
  Serial.print("\t");
  Serial.print(ggy);
  Serial.print("\t");
  Serial.print(ggz);
  Serial.print("\t");
  Serial.print(Gx);
  Serial.print("\t");
  Serial.print(Gy);
  Serial.print("\t");
  
  Serial.println();
}
