#include <SPI.h>

/*
  * CS - to digital pin 10  (SS pin)
  * SDI - to digital pin 11 (MOSI pin)
  * CLK - to digital pin 13 (SCK pin)
*/

int cmd = 9, rst = 8;

void Write_SPI_Command(unsigned char data) {
  digitalWrite(9, LOW);
  SPI.transfer(data);
}
void Write_SPI_Data(unsigned char data) {
  digitalWrite(9, HIGH);
  SPI.transfer(data);
}
void fill(unsigned char fill_Data)
{
  unsigned char m, n;
  for (m = 0; m < 8; m++)
  {
    Write_SPI_Command(0xb0 + m);		//page0-page1
    Write_SPI_Command(0x00);		//low column start address
    Write_SPI_Command(0x10);		//high column start address
    for (n = 0; n < 132; n++)
    {
      Write_SPI_Data(fill_Data);
    }
  }
}
void initOLED() {
  Write_SPI_Command(0xAE);   //display off
  Write_SPI_Command(0x20);	//Set Memory Addressing Mode
  Write_SPI_Command(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
  Write_SPI_Command(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
  Write_SPI_Command(0xc8);	//Set COM Output Scan Direction
  Write_SPI_Command(0x00);//---set low column address
  Write_SPI_Command(0x10);//---set high column address
  Write_SPI_Command(0x40);//--set start line address
  Write_SPI_Command(0x81);//--set contrast control register
  Write_SPI_Command(0x7f);
  Write_SPI_Command(0xa1);//--set segment re-map 0 to 127
  Write_SPI_Command(0xa6);//--set normal display
  Write_SPI_Command(0xa8);//--set multiplex ratio(1 to 64)
  Write_SPI_Command(0x3F);//
  Write_SPI_Command(0xa4);//0xa4,Output follows RAM content;0xa5,Output ignores RAM content
  Write_SPI_Command(0xd3);//-set display offset
  Write_SPI_Command(0x00);//-not offset
  Write_SPI_Command(0xd5);//--set display clock divide ratio/oscillator frequency
  Write_SPI_Command(0xf0);//--set divide ratio
  Write_SPI_Command(0xd9);//--set pre-charge period
  Write_SPI_Command(0x22); //
  Write_SPI_Command(0xda);//--set com pins hardware configuration
  Write_SPI_Command(0x12);
  Write_SPI_Command(0xdb);//--set vcomh
  Write_SPI_Command(0x20);//0x20,0.77xVcc
  Write_SPI_Command(0x8d);//--set DC-DC enable
  Write_SPI_Command(0x14);//
  Write_SPI_Command(0xaf);//--turn on oled panel
}

void setup() {
  // put your setup code here, to run once:
  pinMode(cmd, OUTPUT);
  pinMode(rst, OUTPUT);
  digitalWrite(rst, LOW);
  delay(1);
  digitalWrite(rst, HIGH);
  SPI.begin();
  delay(1);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  //  SPI.setDataMode(SPI_MODE0);
  //  SPI.setBitOrder(MSBFIRST);
  initOLED();
}

void loop() {
  // put your main code here, to run repeatedly:
    fill(0xFF);
    delay(500);
    fill(0x00);
    delay(500);
}
