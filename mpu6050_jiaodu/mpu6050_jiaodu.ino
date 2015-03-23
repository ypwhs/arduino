#include <I2Cdev.h>
#include <MPU6050.h>
#include <Wire.h>

float X_jiaodu=-6;
long s=0,last=0;
float P = 20, D = 0.012;
//====================
MPU6050 accelgyro;
int DIR1 = 7, PWM1 = 6, PWM2 = 5, DIR2 = 4;
int16_t ax, ay, az;
int16_t gx, gy, gz; //存储原始数据
float aax, aay, aaz, ggx, ggy, ggz; //存储量化后的数据
float Ax, Ay, Az; //单位 g(9.8m/s^2)
float Gx, Gy, Gz; //单位 °/s
float Angel_accX, Angel_accY, Angel_accZ; //存储加速度计算出的角度
long LastTime, NowTime, TimeSpan; //用来对角速度积分的
#define LED_PIN 13

bool blinkState = false;
void set(int a)
{
  analogWrite(PWM1, abs(a)); 
  analogWrite(PWM2, abs(a));
  if (a > 0)
  {
    digitalWrite(DIR1, HIGH);
    digitalWrite(DIR2, HIGH);
  }
  else if (a < 0) {
    digitalWrite(DIR1, LOW);
    digitalWrite(DIR2, LOW);
  }
  else {
    digitalWrite(PWM1, LOW);
    digitalWrite(PWM2, LOW);
  }
}
void jishu(){
if(PIND&0X04)s++;
else s--;
}
void setup()//MPU6050的设置都采用了默认值，请参看库文件
{
  digitalWrite(PWM1, LOW);
  digitalWrite(PWM2, LOW);
  pinMode(DIR1, OUTPUT);
  pinMode(DIR2, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(2,INPUT);
  digitalWrite(DIR1, LOW);
  digitalWrite(DIR2, LOW);
  pinMode(LED_PIN, OUTPUT);
  delay(500);
  Wire.begin();
  Serial.begin(57600);
  Serial.println("Initializing I2C device.....");
  accelgyro.initialize();
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failure");
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  Ax = ax / 16384.00;
  Ay = ay / 16384.00;
  Az = az / 16384.00;
  Angel_accZ = atan(Az / sqrt(Ax * Ax + Ay * Ay)) * 180 / 3.14;
  Gx = -Angel_accZ;
  Serial.println(Gx);
  if (!(Gx < 180 && Gx > -180))setup();
  digitalWrite(13,1);
  //attachInterrupt(1, jishu, RISING);
}
char rec;
int speed;
void loop()
{
  //speed=s-last;
  //last=s;
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
  //下面三行就是通过对角速度积分实现各个轴的角度测量，当然假设各轴的起始角度都是0
  Gx = (Gx + ggx * TimeSpan / 1000)*0.995-0.005*Angel_accZ;
  LastTime = NowTime;
  int spd;
  //X_jiaodu-=speed/100.0;
  spd = (Gx - X_jiaodu) * P - ggx * D;
  if (abs(spd)<250)set(spd);
  else {
    set(0);
   /* while (1)
      if (Serial.read() > 0) {
        setup();
        break;
      }
      */
  }
  //==============================
  Serial.print("Ax=");
  Serial.print(Gx);
  Serial.print("\tWx=");
  Serial.print(ggx);
  Serial.print("\tspd=");
  Serial.println(spd);
}
