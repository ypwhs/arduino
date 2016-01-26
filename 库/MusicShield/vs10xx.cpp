/*
 * vs10xx.cpp
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


#include <SPI.h>
#include "vs10xx.h"
#include "pins_config.h"
VS10XX vs1053;

void VS10XX::init(void)
{
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  
  pinMode(VS_XRESET, OUTPUT);
  pinMode(VS_DREQ, INPUT);
  pinMode(VS_XDCS, OUTPUT);
  pinMode(VS_XCS, OUTPUT);
  digitalWrite(VS_XDCS, HIGH);
  digitalWrite(VS_XCS, HIGH);
  
  reset();
}

void VS10XX::initForMidi(void)
{
	//init IO
	pinMode(VS_XRESET, OUTPUT);
	pinMode(VS_DREQ, INPUT);
	pinMode(VS_XDCS, OUTPUT);
	pinMode(VS_XCS, OUTPUT);
	digitalWrite(VS_XDCS, HIGH);
	digitalWrite(VS_XCS, HIGH);
	
	//init SPI
	SPI.begin();
	SPI.setBitOrder(MSBFIRST);
	//SPI.setDataMode(SPI_MODE0);
	SPI.setClockDivider(SPI_CLOCK_DIV16);
	SPI.transfer(0xFF); //transfer a dump
	
	//reset the chip
	putInReset();
	delay(2); //it is a must,2ms
	deselectControlBus();
	deselectDataBus();
	releaseFromReset();

    /* Soft Reset of VS10xx */
	writeRegister(SPI_MODE, 0x0804); /* Newmode, Reset, No L1-2 */
	delay(2);
	while (!readDREQ()); /* wait for startup */

	/* A quick sanity check: write to two registers, then test if we
	 get the same results. Note that if you use a too high SPI
	 speed, the MSB is the most likely to fail when read again. */
	writeRegister(SPI_HDAT0, 0xABAD);
	writeRegister(SPI_HDAT1, 0x1DEA);
	if (readRegister(SPI_HDAT0) != 0xABAD || readRegister(SPI_HDAT1) != 0x1DEA) {
		Serial.print("Something wrong with VS10xx\n");
	}
  
  	writeRegister(SPI_VOL, 0x4040);     //Set volume level
  
  	/* Wait for DREQ */
  	while (!readDREQ());
  
	//load MIDI plugin
	loadPlugin(MIDIPatch,sizeof(MIDIPatch));
}



void VS10XX::writeRegister(unsigned char addressbyte, unsigned int value)
{
  deselectDataBus();
  while (!readDREQ());
  selectControlBus();
  SPI.transfer(VS_WRITE_COMMAND);
  SPI.transfer(addressbyte);
  SPI.transfer(value >> 8);
  SPI.transfer(value & 0xFF);
  deselectControlBus();
}

unsigned int VS10XX::readRegister(unsigned char addressbyte)
{
  unsigned int result = 0;
  deselectDataBus();
  while (!readDREQ());
  selectControlBus(); //XCS = 0
  SPI.transfer(VS_READ_COMMAND); //send read command
  SPI.transfer(addressbyte);    //send register address
  result = SPI.transfer(0xff) << 8;
  result |= SPI.transfer(0xff);
  deselectControlBus();
  return result;
}

void VS10XX::writeData(unsigned char *databuf, unsigned char datalen)
{
  selectDataBus();
  while (!readDREQ());
  while (datalen--)
  {
    SPI.transfer(*databuf++);
  }
  deselectDataBus();
}


// reset for vs10xx
void VS10XX::reset(void)
{
  putInReset();
  delay(2); //it is a must,2ms
  /* Send dummy SPI byte to initialize atmel SPI */
  ////SPIPutCharWithoutWaiting(0xFF);
  SPI.transfer(0xff);
  deselectControlBus();
  deselectDataBus();
  releaseFromReset();
  
  softReset(); //vs10xx soft reset.
}

/** Soft Reset of VS10xx (Between songs) */
void VS10XX::softReset(void)
{
  SPI.setClockDivider(SPI_CLOCK_DIV8);

  /* Soft Reset of VS10xx */
  writeRegister(SPI_MODE, 0x0804); /* Newmode, Reset, No L1-2 */
  delay(2);
  while (!readDREQ()); /* wait for startup */

  /* A quick sanity check: write to two registers, then test if we
     get the same results. Note that if you use a too high SPI
     speed, the MSB is the most likely to fail when read again. */
  writeRegister(SPI_HDAT0, 0xABAD);
  writeRegister(SPI_HDAT1, 0x1DEA);
  if (readRegister(SPI_HDAT0) != 0xABAD || readRegister(SPI_HDAT1) != 0x1DEA) {
  	Serial.print("Something wrong with VS10xx\n");
  }
  
  //setup I2S (see page77 of the datasheet of vs1053 )
  //set GPIO0 as output
  //writeRegister(SPI_WRAMADDR, 0xc017);
  //writeRegister(SPI_WRAM, 0x00f0);
  //enable I2S (MCLK enabled, 48kHz sample rate)
  //writeRegister(SPI_WRAMADDR, 0xc040);
  //writeRegister(SPI_WRAM, 0x000C);
  
  writeRegister(SPI_CLOCKF,0Xc000);   //Set the clock
  writeRegister(SPI_AUDATA,0xbb81);   //samplerate 48k,stereo
  writeRegister(SPI_BASS, 0x0055);    //set accent
  writeRegister(SPI_VOL, 0x4040);     //Set volume level
  
  /* Wait for DREQ */
  while (!readDREQ());
}



/*
 **@ function name: loadMidiPatch
 **@ usage:load a software patch for vs10xx
 **@ input:none
 **@ retval:none
*/

void VS10XX::loadPlugin(const unsigned short *plugin,int length)
{
	int i = 0;
	while (i<length) {
	  unsigned short addr, n, val;
	  addr = plugin[i++];
	  n = plugin[i++];
	  if (n & 0x8000U) { //RLE run, replicate n samples 
		n &= 0x7FFF;
		val = plugin[i++];
		while (n--) {
		  writeRegister(addr, val);
		}
	  } else {			 //Copy run, copy n samples 
		while (n--) {
		  val = plugin[i++];
		  writeRegister(addr, val);
		}
	  }
	}

}
/****************The end***********************************************/
