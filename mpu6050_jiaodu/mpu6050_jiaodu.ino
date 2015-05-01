#include <I2Cdev.h>
#include <MPU6050.h>
#include <Wire.h>

float X_jiaodu=-6;
long s=0,last=0;
float P = 20, D = 0.012; //PID调参数位置
//====================
MPU6050 accelgyro;
int DIR1 = 7, PWM1 = 6, PWM2 = 5, DIR2 = 4; //驱动板引脚定义
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
  //正交解码
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
  pinMode(2,INPUT); //编码器引脚初始化
  digitalWrite(DIR1, LOW);
  digitalWrite(DIR2, LOW); //首先默认输出低电平防止电机转动
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
  Angel_accZ = atan(Az / sqrt(Ax * Ax + Ay * Ay)) * 180 / 3.14; //这部分是空间几何知识
  Gx = -Angel_accZ; //首先初始化的时候Gx先设置为加速度换算出来的角度
  Serial.println(Gx);
  if (!(Gx < 180 && Gx > -180))setup(); //有问题的话就重新初始化一下
  digitalWrite(13,1);
  //attachInterrupt(1, jishu, RISING); //这里是编码器计数中断
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
  LastTime = NowTime;
  //下面这行是互补滤波,短期来看陀螺仪很准,所以比重是99.5%,但是陀螺仪有累积误差,所以需要加速度来互补,这里用减号是因为加速度算出来方向是反的
  Gx = (Gx + ggx * TimeSpan / 1000)*0.995-0.005*Angel_accZ;
  int spd;
  spd = (Gx - X_jiaodu) * P - ggx * D;
  if (abs(spd)<250)set(spd);
  else {
    set(0);
   /* while (1) //等待串口传回数据再接着跑
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
