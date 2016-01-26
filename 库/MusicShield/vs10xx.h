/*
 * vs10xx.h
 * A library for MusicShield
 *
 * Copyright (c) 2015 waveshare.net
 * Website    : waveshare.net
 * Author      :still falling leaves
 * Create Time: May 2015
 * Change Log :
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#ifndef VS10XX_H
#define VS10XX_H
#include <SPI.h>
#include "pins_config.h"

/** VS10xx SCI Write Command byte is 0x02 */
#define VS_WRITE_COMMAND 0x02
/** VS10xx SCI Read Command byte is 0x03 */
#define VS_READ_COMMAND 0x03


#define SPI_MODE	0x0   /**< VS10xx register */
#define SPI_STATUS	0x1   /**< VS10xx register */
#define SPI_BASS	0x2   /**< VS10xx register */
#define SPI_CLOCKF	0x3   /**< VS10xx register */
#define SPI_DECODE_TIME	0x4   /**< VS10xx register */
#define SPI_AUDATA	0x5   /**< VS10xx register */
#define SPI_WRAM	0x6   /**< VS10xx register */
#define SPI_WRAMADDR	0x7   /**< VS10xx register */
#define SPI_HDAT0	0x8   /**< VS10xx register */
#define SPI_HDAT1	0x9   /**< VS10xx register */
#define SPI_AIADDR	0xa   /**< VS10xx register */
#define SPI_VOL		0xb   /**< VS10xx register */
#define SPI_AICTRL0	0xc   /**< VS10xx register */
#define SPI_AICTRL1	0xd   /**< VS10xx register */
#define SPI_AICTRL2	0xe   /**< VS10xx register */
#define SPI_AICTRL3	0xf   /**< VS10xx register */

#define SM_DIFF		0x01   /**< VS10xx register */
#define SM_JUMP		0x02   /**< VS10xx register */
#define SM_RESET	0x04   /**< VS10xx register */
#define SM_OUTOFWAV	0x08   /**< VS10xx register */
#define SM_PDOWN	0x10   /**< VS10xx register */
#define SM_TESTS	0x20   /**< VS10xx register */
#define SM_STREAM	0x40   /**< VS10xx register */
#define SM_PLUSV	0x80   /**< VS10xx register */
#define SM_DACT		0x100   /**< VS10xx register */
#define SM_SDIORD	0x200   /**< VS10xx register */
#define SM_SDISHARE	0x400   /**< VS10xx register */
#define SM_SDINEW	0x800   /**< VS10xx register */
#define SM_ADPCM        0x1000   /**< VS10xx register */
#define SM_ADPCM_HP     0x2000   /**< VS10xx register */

class VS10XX : public SPIClass
{
public:
  void selectControlBus(void)   { digitalWrite(VS_XCS, LOW);}
  void deselectControlBus(void) { digitalWrite(VS_XCS, HIGH);}
  void selectDataBus(void)      { digitalWrite(VS_XDCS, LOW);}
  void deselectDataBus(void)    { digitalWrite(VS_XDCS, HIGH);}
  void putInReset(void)         { digitalWrite(VS_XRESET, LOW);}
  void releaseFromReset(void)   { digitalWrite(VS_XRESET, HIGH);}
  char readDREQ(void) { return digitalRead(VS_DREQ);}
  
  void writeRegister(unsigned char addressbyte, unsigned int value);
  unsigned int readRegister(unsigned char addressbyte); /** Read the 16-bit value of a VS10xx register */
  void writeData(unsigned char *databuf, unsigned char = 32); //write 32 bytes to vs1053
  
  
  void init(void);
  void initForMidi(void);
  void reset(void); //reset for vs10xx
  void softReset(void); /* Soft Reset of VS10xx (Between songs) */
  void loadPlugin(const  unsigned short *plugin,int length);
private:

};




const unsigned short recPlugin[40] = { /* Compressed plugin for recording*/
  0x0007, 0x0001, 0x8010, 0x0006, 0x001c, 0x3e12, 0xb817, 0x3e14, /*    0 */
  0xf812, 0x3e01, 0xb811, 0x0007, 0x9717, 0x0020, 0xffd2, 0x0030, /*    8 */
  0x11d1, 0x3111, 0x8024, 0x3704, 0xc024, 0x3b81, 0x8024, 0x3101, /*   10 */
  0x8024, 0x3b81, 0x8024, 0x3f04, 0xc024, 0x2808, 0x4800, 0x36f1, /*   18 */
  0x9811, 0x0007, 0x0001, 0x8028, 0x0006, 0x0002, 0x2a00, 0x040e,
};

//software patch for MIDI Play
const unsigned short MIDIPatch[28]={
/*if you don't let GPIO1 = H,please send this patch by spi*/
0x0007, 0x0001, 0x8050, 0x0006, 0x0014, 0x0030, 0x0715, 0xb080, /* 0 */
0x3400, 0x0007, 0x9255, 0x3d00, 0x0024, 0x0030, 0x0295, 0x6890, /* 8 */
0x3400, 0x0030, 0x0495, 0x3d00, 0x0024, 0x2908, 0x4d40, 0x0030, /* 10 */
0x0200, 0x000a, 0x0001, 0x0050,
};



#endif
