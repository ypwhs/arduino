#include <Wire.h>
#include <Kalman.h>

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
  
}
unsigned long lasttime = 0;
Kalman kalmanRoll;
Kalman kalmanPitch;

void loop()
{

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
  
  float roll = acos(sqrt(xz)/accG) * RAD_TO_DEG;
  float pitch = acos(sqrt(yz)/accG) * RAD_TO_DEG;
  if(ay>0)roll=-roll;
  if(ax<0)pitch=-pitch;
  
  float dt = (double)(micros() - lasttime) / 1000000.0;

  float roll2 = kalmanRoll.getAngle(roll, -gyroX, dt);
  float pitch2 = kalmanPitch.getAngle(pitch, -gyroX, dt);

  Serial.print(roll2);
  Serial.print("\t");
  Serial.print(pitch2);
  Serial.print("\t");
  
  // End of line
  Serial.println("");
  //  delay(100);
}
