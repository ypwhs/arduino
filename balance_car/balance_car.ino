#include <MPU6050.h>
#include <Wire.h>

float X_jiaodu = 3;
long s = 0, last = 0;
MPU6050 accelgyro;

int P1 = 5, P2 = 6, P3 = 9, P4 = 10; //驱动板引脚定义
int16_t ax, ay, az;
int16_t gx, gy, gz; //存储原始数据
float aax, aay, aaz, ggx, ggy, ggz; //存储量化后的数据
float Ax, Ay, Az; //单位 g(9.8m/s^2)
float Gx, Gy, Gz; //单位 °/s
float Angel_accX, Angel_accY, Angel_accZ; //存储加速度计算出的角度
long LastTime, NowTime, TimeSpan; //用来对角速度积分的
#define LED_PIN 13

bool blinkState = false;
void setSpeed(int a)
{
  int aa = abs(a);
  if (a < 0)
  {
    analogWrite(P1, aa);  digitalWrite(P2, LOW);
    digitalWrite(P3, LOW); analogWrite(P4, aa);
  }
  else if (a > 0) {
    digitalWrite(P1, LOW); analogWrite(P2, aa);
    analogWrite(P3, aa);  digitalWrite(P4, LOW);
  }
  else {
    digitalWrite(P1, LOW); digitalWrite(P2, LOW);
    digitalWrite(P3, LOW); digitalWrite(P4, LOW);
  }
}
void jishu() { //正交解码
  if (PIND & 0X04)s--;
  else s++;
}
void setup() {
  //电机初始化,首先默认输出低电平防止电机转动
  digitalWrite(P1, LOW);
  digitalWrite(P2, LOW);
  digitalWrite(P3, LOW);
  digitalWrite(P4, LOW);
  pinMode(P1, OUTPUT);
  pinMode(P2, OUTPUT);
  pinMode(P3, OUTPUT);
  pinMode(P4, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  delay(500);
  Wire.begin();
  Serial.begin(115200);
  Serial.println("Initializing I2C device.....");
  accelgyro.initialize();
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failure");
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  Ax = ax / 16384.00;
  Ay = ay / 16384.00;
  Az = az / 16384.00;
  Angel_accZ = atan(Az / sqrt(Ax * Ax + Ay * Ay)) * 180 / 3.14; //这部分是空间几何知识
  Gx = -Angel_accZ; //首先初始化的时候Gx先设置为加速度换算出来的角度
  Serial.println(Gx);
  if (!(Gx < 180 && Gx > -180))setup(); //有问题的话就重新初始化一下
  digitalWrite(13, 1);
}
char rec;
int speed, pwm;
int pwm2 = 0;

float P = 10, D = 0.3; //PID参数
void loop()
{
  speed = s - last;
  last = s;
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz); //获取三个轴的加速度和角速度
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
  //下面这行是互补滤波,短期来看陀螺仪很准,所以比重是99.5%,但是陀螺仪有累积误差,所以需要加速度来互补
  Gx = (Gx + ggy * TimeSpan / 1000) * 0.995 + 0.005 * (-Angel_accZ);

  pwm = (Gx - X_jiaodu) * P + ggy * D;
  setSpeed(-(pwm + pwm2) / 2);
  pwm2 = pwm;
}
