#include <Wire.h>
#include <Kalman.h>
#include <TimerOne.h>

#define    MPU9250_ADDRESS            0x68
#define    MAG_ADDRESS                0x0C

#define    GYRO_FULL_SCALE_250_DPS    0x00
#define    GYRO_FULL_SCALE_500_DPS    0x08
#define    GYRO_FULL_SCALE_1000_DPS   0x10
#define    GYRO_FULL_SCALE_2000_DPS   0x18

#define    ACC_FULL_SCALE_2_G        0x00
#define    ACC_FULL_SCALE_4_G        0x08
#define    ACC_FULL_SCALE_8_G        0x10
#define    ACC_FULL_SCALE_16_G       0x18

void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data)
{
  // Set register address
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.endTransmission();

  // Read Nbytes
  Wire.requestFrom(Address, Nbytes);
  uint8_t index = 0;
  while (Wire.available())
    Data[index++] = Wire.read();
}


// Write a byte (Data) in device (Address) at register (Register)
void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data)
{
  // Set register address
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.write(Data);
  Wire.endTransmission();
}

// Initial time
long int ti;
volatile bool intFlag = false;

// Initializations
void setup()
{
  // Arduino initializations
  Wire.begin();
  Serial.begin(115200);

  // Set accelerometers low pass filter at 5Hz
  I2CwriteByte(MPU9250_ADDRESS, 29, 0x06);
  // Set gyroscope low pass filter at 5Hz
  I2CwriteByte(MPU9250_ADDRESS, 26, 0x06);


  // Configure gyroscope range
  I2CwriteByte(MPU9250_ADDRESS, 27, GYRO_FULL_SCALE_1000_DPS);
  // Configure accelerometers range
  I2CwriteByte(MPU9250_ADDRESS, 28, ACC_FULL_SCALE_4_G);
  // Set by pass mode for the magnetometers
  I2CwriteByte(MPU9250_ADDRESS, 0x37, 0x02);

  // Request continuous magnetometer measurements in 16 bits
  I2CwriteByte(MAG_ADDRESS, 0x0A, 0x16);

  //I2CwriteByte(MPU9250_ADDRESS, 106, 1 << 6);
  
  pinMode(13, OUTPUT);
  Timer1.initialize(10000);         // initialize timer1, and set a 1/2 second period
  Timer1.attachInterrupt(callback);  // attaches callback() as a timer overflow interrupt


  // Store initial time
  ti = millis();
}


// Counter
long int cpt = 0;

void callback()
{
  intFlag = true;
  digitalWrite(13, digitalRead(13) ^ 1);
}


unsigned long nLastTime = 0; //上一次读数的时间
float fLastRoll = 0.0f; //上一次滤波得到的Roll角
float fLastPitch = 0.0f; //上一次滤波得到的Pitch角
Kalman kalmanRoll; //Roll角滤波器
Kalman kalmanPitch; //Pitch角滤波器

// Main loop, read and display data
void loop()
{
  while (!intFlag);
  intFlag = false;

  // Display time
//  Serial.print (millis() - ti, DEC);
//  Serial.print ("\t");


  // _______________
  // ::: Counter :::

  // Display data counter
  //  Serial.print (cpt++,DEC);
  //  Serial.print ("\t");



  // ____________________________________
  // :::  accelerometer and gyroscope :::

  // Read accelerometer and gyroscope
  uint8_t Buf[14];
  I2Cread(MPU9250_ADDRESS, 0x3B, 14, Buf);

  // Create 16 bits values from 8 bits data

  // Accelerometer
  int16_t ax = -(Buf[0] << 8 | Buf[1]);
  int16_t ay = -(Buf[2] << 8 | Buf[3]);
  int16_t az = Buf[4] << 8 | Buf[5];

  // Gyroscope
  int16_t gx = -(Buf[8] << 8 | Buf[9]);
  int16_t gy = -(Buf[10] << 8 | Buf[11]);
  int16_t gz = Buf[12] << 8 | Buf[13];


  // Display values
/*
  // Accelerometer
  Serial.print (ax, DEC);
  Serial.print ("\t");
  Serial.print (ay, DEC);
  Serial.print ("\t");
  Serial.print (az, DEC);
  Serial.print ("\t");

  // Gyroscope
  Serial.print (gx, DEC);
  Serial.print ("\t");
  Serial.print (gy, DEC);
  Serial.print ("\t");
  Serial.print (gz, DEC);
  Serial.print ("\t");
*/

  // _____________________
  // :::  Magnetometer :::


  // Read register Status 1 and wait for the DRDY: Data Ready

  uint8_t ST1;
  do
  {
    I2Cread(MAG_ADDRESS, 0x02, 1, &ST1);
  }
  while (!(ST1 & 0x01));

  // Read magnetometer data
  uint8_t Mag[7];
  I2Cread(MAG_ADDRESS, 0x03, 7, Mag);


  // Create 16 bits values from 8 bits data

  // Magnetometer
  int16_t mx = -(Mag[3] << 8 | Mag[2]);
  int16_t my = -(Mag[1] << 8 | Mag[0]);
  int16_t mz = -(Mag[5] << 8 | Mag[4]);

/*
  // Magnetometer
  Serial.print (mx, DEC);
  Serial.print ("\t");
  Serial.print (my, DEC);
  Serial.print ("\t");
  Serial.print (mz, DEC);
  Serial.print ("\t");
*/

  //calculate
  float accX = ax * 4.0 / 32768 ;
  float accY = ay * 4.0 / 32768;
  float accZ = az * 4.0 / 32768;
  
  float gyroX = gx * 1000.0 / 32768;
  float gyroY = gy * 1000.0 / 32768;
  float gyroZ = gz * 1000.0 / 32768;
  
  float accG = sqrt(accX * accX + accY * accY + accZ * accZ);
  float yz = accZ * accZ + accY * accY;
  float xz = accX * accX + accZ * accZ;
  
  float roll = acos(sqrt(xz)/accG);
  float pitch = acos(sqrt(yz)/accG);
  if(ay>0)roll=-roll;
  if(ax<0)pitch=-pitch;
  
  //计算两次测量的时间间隔dt，以秒为单位
  unsigned long nCurTime = micros();
  float dt = (double)(nCurTime - nLastTime) / 1000000.0;
  //对Roll角和Pitch角进行卡尔曼滤波
  float fNewRoll = kalmanRoll.getAngle(roll, gyroX, dt);
  float fNewPitch = kalmanPitch.getAngle(pitch, gyroY, dt);

  Serial.print(fNewRoll*1000);
  Serial.print("\t");
  Serial.print(fNewPitch*1000);
  Serial.print("\t");
  // End of line
  Serial.println("");
  //  delay(100);
}
