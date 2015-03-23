#define uchar unsigned char
#define uint  unsigned int
int DIN = 6, CS = 5, CLK = 4;
char a[8]={0x01,0,0,0,0,0,0,0};
void Write_Max7219_byte(uchar DATA)
{
  uchar i;
  digitalWrite(CS, 0);
  for (i = 8; i >= 1; i--)
  {
    digitalWrite(CLK, 0);
    digitalWrite(DIN, DATA & 0x80);
    DATA = DATA << 1;
    digitalWrite(CLK, 1);
  }
}

void Write_Max7219(uchar address,uchar dat)
{ 
     digitalWrite(CS,0);
	 Write_Max7219_byte(address);           //写入地址，即数码管编号
     Write_Max7219_byte(dat);               //写入数据，即数码管显示数字 
	 digitalWrite(CS,1);                        
}

void Init_MAX7219(void)
{
 Write_Max7219(0x09, 0x00);       //译码方式：BCD码
 Write_Max7219(0x0a, 0x03);       //亮度 
 Write_Max7219(0x0b, 0x07);       //扫描界限；8个数码管显示
 Write_Max7219(0x0c, 0x01);       //掉电模式：0，普通模式：1
 Write_Max7219(0x0f, 0x00);       //显示测试：1；测试结束，正常显示：0
}

void setup() {
  // put your setup code here, to run once:
  pinMode(DIN, OUTPUT);
  pinMode(CS, OUTPUT);
  pinMode(CLK, OUTPUT);
  Init_MAX7219();
  Serial.begin(115200);
}
void loop() {
  // put your main code here, to run repeatedly:
uint i;

for(i=0;i<8;i++){Write_Max7219(i+1,a[i]);Serial.print(a[i]);}
delay(500);

for(i=0;i<8;i++)if(!(a[i]>>8)){a[i]*=2;a[i]+=1;break;}

}
