#include <I2Cdev.h>
#include <MPU6050.h>
#include <Wire.h>

float X_jiaodu_o = -5.8;
float X_jiaodu = -5.8;
float Roffset=0;
long s=0,last=0;
float P = 45, D = 0.2;
float PS=0.04,DS = 0.03;
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
//  if(abs(a)<abs(Roffset))
//  {
//    digitalWrite(PWM1, LOW);
//    digitalWrite(PWM2, LOW);
//    return;
//  }
  if (a > 0)
  {
    digitalWrite(DIR1, HIGH);
    digitalWrite(DIR2, HIGH);
    analogWrite(PWM1, a+Roffset);
    analogWrite(PWM2, a-Roffset);
  }
  else if (a < 0) {
    digitalWrite(DIR1, LOW);
    digitalWrite(DIR2, LOW);
    analogWrite(PWM1, -a+Roffset);
    analogWrite(PWM2, -a-Roffset);
  }
  else {
    digitalWrite(PWM1, LOW);
    digitalWrite(PWM2, LOW);
  }
}
void fangxiang(){
  if(PIND&0X04){
    s++;
  }
  else {
  s--;
  }
}
void setup()//MPU6050的设置都采用了默认值，请参看库文件
{
  pinMode(2,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);
  attachInterrupt(1, fangxiang, FALLING);
  digitalWrite(PWM1, LOW);
  digitalWrite(PWM2, LOW);
  pinMode(DIR1, OUTPUT);
  pinMode(DIR2, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  digitalWrite(DIR1, LOW);
  digitalWrite(DIR2, LOW);
  pinMode(LED_PIN, OUTPUT);
  Wire.begin();
  Serial.begin(115200);
  Serial.println("Initializing I2C device.....");
  delay(100);
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
}
char rec;
int speed=0;float spd=0,lastspd=0,aspd=0;
long nowtime2=0,lasttime2=0,timespan2=0;
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
//  Angel_accX = atan(Ax / sqrt(Az * Az + Ay * Ay)) * 180 / 3.14;
//  Angel_accY = atan(Ay / sqrt(Ax * Ax + Az * Az)) * 180 / 3.14;
  Angel_accZ = atan(Az / sqrt(Ax * Ax + Ay * Ay)) * 180 / 3.14;
  //==========以下三行是对角速度做量化==========
//  ggx = gx / 131.00;
  ggy = gy / 131.00;
//  ggz = gz / 131.00;
  //===============以下是对角度进行积分处理====== ==========
  NowTime = millis(); //获取当前程序运行的毫秒数
  TimeSpan = NowTime - LastTime; //积分时间这样算不是很严谨
  //下面三行就是通过对角速度积分实现各个轴的角度测量，当然假设各轴的起始角度都是0
  Gx = (Gx + ggy * TimeSpan / 1000)*0.995-0.005*Angel_accZ;
  LastTime = NowTime;

  nowtime2 = millis();
  if(nowtime2-lasttime2>15)
  {
    timespan2=nowtime2-lasttime2;
    spd=(s-last)*1000/timespan2;
    aspd=spd-lastspd;
    lastspd=spd;
    last=s;
    lasttime2=nowtime2;
  }
  
  speed = (Gx - X_jiaodu) * P - ggx * D + spd*PS - aspd*DS;
  if (abs(speed)<254)
  set(-speed);
  else {
    //set(0);
   /* while (1)
      if (Serial.read() > 0) {
        setup();
        break;
      }
      */
  }
  //==============================
  if(Serial.available())
  {
    char bufchar = Serial.read();
    Serial.print(bufchar);
    if(bufchar == 'w')X_jiaodu = X_jiaodu_o+2;
    else if(bufchar == 's')X_jiaodu = X_jiaodu_o-2;
    else if(bufchar == 'a'){
      Roffset=40;
      X_jiaodu = X_jiaodu_o+2;
    }
    else if(bufchar == 'd'){
      Roffset=-40;
      X_jiaodu = X_jiaodu_o+2;
    }else if(bufchar == 'x'){
      Roffset=0;
      X_jiaodu = X_jiaodu_o;
    }
    Serial.println();
  }
//
  Serial.print("Ax=");
  Serial.print(Gx-X_jiaodu);
  Serial.print("\tSpeed=");
  Serial.print(spd);
  Serial.print("\taspd=");
  Serial.print(aspd);
  Serial.print("\tPWM=");
  Serial.println(speed);
}
