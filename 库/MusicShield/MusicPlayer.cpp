/*
 * MusicPlayer.cpp
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

#include <SD.h>
#include <MusicPlayer.h>
#include  <avr/pgmspace.h>


// change this to match your SD shield or module;
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8

static unsigned char fileBuf[32];
songtype list[15];            //song list
int ledcount;
key keys[5];


VS10XX VS1053;
Sd2Card     card;
SdVolume    volume;
SdFile root;
SdFile file;
playingstatetype playingState;
ctrlStatetype ctrlState;



/* PCM file Header */
unsigned char pcmHeader[44] = {
  'R', 'I', 'F', 'F',
  0xFF, 0xFF, 0xFF, 0xFF,
  'W', 'A', 'V', 'E',
  'f', 'm', 't', ' ',
  0x10, 0, 0, 0,          /* 16 */
  0x1, 0,                 /* PCM */
  0x1, 0,                 /* chan */
  0x40, 0x1F, 0x0, 0x0,   /* sampleRate */
  0x80, 0x3E, 0x0, 0x0,   /* byteRate */
  2, 0,                   /* blockAlign */
  0x10, 0,                /* bitsPerSample */
  'd', 'a', 't', 'a',
  0xFF, 0xFF, 0xFF, 0xFF
};

/**************************************************************/
ISR(TIMER1_OVF_vect)          //Timer1 Service
{
	for(int i=0;i<5;i++)
	{
		keys[i].scanKey();
	}
	
	if (playingState == PS_PAUSE) digitalWrite(LED, HIGH);
	else if (playingState == PS_IDLE) digitalWrite(LED,LOW);
	else 
	if (ledcount-- == 0)
	{
		digitalWrite(LED,!digitalRead(LED));
		ledcount = 500;
	}
}


/**************************************************************/
#define RESOLUTION 65536    // Timer1 is 16 bit
void MusicPlayer::initTimer1(void)        //initialize Timer1 to 100us overflow
{
  TCCR1A = 0;                 // clear control register A
  TCCR1B = _BV(WGM13);        // set mode as phase and frequency correct pwm, stop the timer

  long cycles;
  long microseconds = 500;   //setup microseconds here
  unsigned char clockSelectBits;
  cycles = (F_CPU / 2000000) * microseconds;                                // the counter runs backwards after TOP, interrupt is at BOTTOM so divide microseconds by 2
  if (cycles < RESOLUTION)              clockSelectBits = _BV(CS10);              // no prescale, full xtal
  else if ((cycles >>= 3) < RESOLUTION) clockSelectBits = _BV(CS11);              // prescale by /8
  else if ((cycles >>= 3) < RESOLUTION) clockSelectBits = _BV(CS11) | _BV(CS10);  // prescale by /64
  else if ((cycles >>= 2) < RESOLUTION) clockSelectBits = _BV(CS12);              // prescale by /256
  else if ((cycles >>= 2) < RESOLUTION) clockSelectBits = _BV(CS12) | _BV(CS10);  // prescale by /1024
  else        cycles = RESOLUTION - 1, clockSelectBits = _BV(CS12) | _BV(CS10);  // request was out of bounds, set as maximum

  ICR1 = cycles;
  TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));
  TCCR1B |= clockSelectBits;                                          // reset clock select register, and starts the clock

  TIMSK1 = _BV(TOIE1);
  TCNT1 = 0;
  sei();                      //enable global interrupt
}


/**************************************************************/
void MusicPlayer::initIO(void)
{
  pinMode(SD_CS_PIN, OUTPUT);
  digitalWrite(SD_CS_PIN, HIGH);
  
  pinMode(VS_XCS, OUTPUT);
  digitalWrite(VS_XCS, HIGH);

  pinMode(LED, OUTPUT);
  digitalWrite(LED,LOW);
  	
  //keys input
  pinMode(KEY_VD, INPUT);
  pinMode(KEY_BK, INPUT);
  pinMode(KEY_PS, INPUT);
  pinMode(KEY_NT, INPUT);
  pinMode(KEY_VU, INPUT);
  //internal pull-up
  digitalWrite(KEY_VD, HIGH);
  digitalWrite(KEY_BK, HIGH);
  digitalWrite(KEY_PS, HIGH);
  digitalWrite(KEY_NT, HIGH);
  digitalWrite(KEY_VU, HIGH);
}


/**************************************************************/
void MusicPlayer::initSD(void)
{

  char i = 0;
  dir_t p;
  char name[13];

  /* init sd card */
  if(!card.init(SPI_FULL_SPEED, SD_CS_PIN))   //SPI_FULL_SPEED
  { 
    Serial.println("SD initialization failed.");
	while(1);
  }
  if (!volume.init(card))
  {
    Serial.println("Could not find FAT16/FAT32 partition.");
    while(1);
  }
  root.openRoot(volume);
  Serial.println("---- songs in TF card (root dir) ----");
  while(root.readDir(&p) != 0)
  {
    SdFile::dirName(p, name);
	char *s = name;
	s = strupr(s);
	if(strstr(s,".MP3")||strstr(s,".WAV")||strstr(s,".WMA")||strstr(s,".OGG")
             ||strstr(s,".AAC"))
    {
		uint32_t curPos = root.curPosition();
		uint16_t index = (curPos - 32) >> 5;
		if (file.open(&root,index, O_READ))
		{
			file.close();
			strncpy(list[i].name, name, 13);
			list[i].index = index;
			Serial.println(list[i++].name);
		}	
    }
  }
  Serial.println();
  MaxSong = (i-1);
}

/**************************************************************/
void MusicPlayer::begin(void)
{
	keys[0].setKey(KEY_PS,CS_PLAYPAUSE);
  	keys[1].setKey(KEY_VD,CS_DOWN);
	keys[2].setKey(KEY_VU,CS_UP);
  	keys[3].setKey(KEY_BK,CS_PREV);
  	keys[4].setKey(KEY_NT,CS_NEXT);
	
  	initIO();
  	initSD();
  	VS1053.init();
  	initTimer1();  // init timer1 
}


/**************************************************************/
void key::setKey(unsigned char io,unsigned char return_value)
{
  _io = io;
  _return_value = return_value;
  _key_st = KS_IDLE;
  _keycnt = 0;
}

/**************************************************************/
void key::scanKey(void)
{
  switch (_key_st)
  {
  case KS_IDLE:
    if (digitalRead(_io) == LOW) _key_st = KS_PRESS;
    _keycnt = 0;
    break;
  case KS_PRESS:
    if (digitalRead(_io) == LOW)
    {
      	_keycnt++;
    } else
    {
		if (_keycnt > 20)   //10ms, short press
      	{
        	ctrlState = (ctrlStatetype)_return_value;
      	}
      	if (_keycnt > 1000) //50 ms,long press
      	{
        	if((ctrlStatetype)_return_value == CS_PLAYPAUSE)ctrlState = CS_RECORDING;
      	}
     	_keycnt = 0;
      	_key_st = KS_IDLE;

		if(ctrlState == CS_PLAYPAUSE)
		{
			if(playingState == PS_PAUSE)playingState = PS_PLAY;
			else playingState =PS_PAUSE;
			ctrlState = CS_EMPTY;
		}
		if(ctrlState == CS_RECORDING)
		{
			playingState = PS_RECORDING;
			ctrlState = CS_EMPTY; 
		}
    }	
    break;
  default:
    break;
  }
}


/**************************************************************/
void MusicPlayer::play()
{

	unsigned char bytes;        // How many bytes in buffer left
	unsigned char n;
	
	SPI.setClockDivider(SPI_CLOCK_DIV2);
	if(file.open(&root,list[songIndex].index,O_READ))
  	{
		Serial.print("Playing ");	
		Serial.println(list[songIndex].name);		
  	}
	else
	{
		Serial.print("Could not open: ");
		Serial.println(list[songIndex].name);
		songIndex ++;
		while(1);
	}

	playingState = PS_PLAY;
	while(bytes = file.read(fileBuf,sizeof(fileBuf)))
	{      
        // actual audio data gets sent to VS10xx.
       	VS1053.writeData(fileBuf,bytes);
        
		switch (playingState)
		{ 
			case PS_PLAY:
				switch(ctrlState)
				{	
					case CS_UP:
						Vol += 10;
						if (Vol > MaxVol)Vol = MaxVol;
						VS1053.writeRegister(SPI_VOL, Vol*0x101);     //Set volume level
						ctrlState = CS_EMPTY;
						break;
					case CS_DOWN:
						Vol -= 10;
						if (Vol < 0)Vol = 0;
						VS1053.writeRegister(SPI_VOL,Vol*0x101);      //Set volume level
						ctrlState = CS_EMPTY;
						break;
					case CS_NEXT:
						if (++songIndex > MaxSong)songIndex = 0;
						file.close();
						VS1053.softReset();
						ctrlState = CS_EMPTY;
						return;
					case CS_PREV:
						if (--songIndex < 0)songIndex = MaxSong;
						file.close();
						VS1053.softReset();
						ctrlState = CS_EMPTY;
						return;
					default:
						break;	
				}
				break;
			case PS_PAUSE:
				while(playingState == PS_PAUSE);         //Pause
				break;
			case PS_RECORDING:
				file.close();
				VS1053.softReset();
			    return;
		}
				
	}
	file.close();
	VS1053.softReset();
	Serial.println("[done!]");
	songIndex++;

}




/**************************************************************/
void Set32(unsigned char *d, unsigned int n) {
  int i;
  for (i=0; i<4; i++) {
    *d++ = (unsigned char)n;
    n >>= 8;
  }
}

/**************************************************************/
void MusicPlayer::recording(char *recfile) {
    unsigned long fileSize = 0;
    unsigned int n;

    VS1053.writeRegister(SPI_BASS, 0x0055);    //set accent
    VS1053.writeRegister(SPI_AICTRL0,8000);    //samplerate 8k
    VS1053.writeRegister(SPI_AICTRL1,0);       //record gain
    VS1053.writeRegister(SPI_AICTRL2,0);       //Set the gain maximum,65536=64X
    VS1053.writeRegister(SPI_AICTRL3,6);       //PCM Mode ,left channel
    VS1053.writeRegister(SPI_CLOCKF,0x2000);   //set clock
    
    VS1053.writeRegister(SPI_MODE,0x1804);   //mic,Initialize recording
    delay(2);
    
    VS1053.loadPlugin(recPlugin,sizeof(recPlugin)/sizeof(recPlugin[0]));

	SPI.setClockDivider(SPI_CLOCK_DIV2);
	file.remove(&root,recfile);
    
	if(file.open(&root,recfile,O_WRITE|O_CREAT) == NULL)
  	{
		Serial.println("Rec open fail");
		return;
  	}
	playingState == PS_RECORDING;
	Serial.println("Recording ...");	
	
	while(playingState != PS_PAUSE)
	{
		if ((n = VS1053.readRegister(SPI_HDAT1)) > 0)
		{
                //Make little-endian conversion for 16-bit PCM .WAV files 
               for (char i=0; i<n;i++) {
                   unsigned int w = VS1053.readRegister(SPI_HDAT0);
                   file.write((unsigned char)(w & 0xFF));
                   file.write((unsigned char)(w >> 8));
                }
               fileSize += 2*n;
		}
	}
	 	/* Update file sizes for an RIFF PCM .WAV file */
    	file.seekSet(0);
        Set32(pcmHeader+4, fileSize-8);
        Set32(pcmHeader+40, fileSize-36);
        file.write(pcmHeader, sizeof(pcmHeader));
        file.close();

        /* Finally, reset the VS10xx software, including realoading the
             patches package, to make sure everything is set up properly. */
        VS1053.softReset();
}


/**********************************Midi Player Part***************************/
void MusicPlayer::beginMidi(void)
{
  //Init VS105B in Midi Format
  Serial.print("Init vs10xx in MIDI format...");
  VS1053.initForMidi();
  Serial.print("done\r\n");
}

void MusicPlayer::midiWriteData(char cmd, char high, char low)
{
  while(!digitalRead(VS_DREQ));
  digitalWrite(VS_XDCS, LOW);
  midiSendByte(cmd);
  if((cmd & 0xF0) <= 0xB0 || (cmd & 0xF0) >= 0xE0)
  {
    midiSendByte(high);
    midiSendByte(low);
  }
  else
  {
    midiSendByte(high);
  }
  digitalWrite(VS_XDCS, HIGH);
}

void MusicPlayer::midiNoteOn(char channel, char note, char rate)
{
  midiWriteData((0x90 | channel), note, rate);
}

void MusicPlayer::midiNoteOff(char channel, char note, char rate)
{
  midiWriteData((0x80 | channel), note, rate);
}

void MusicPlayer::midiSendByte(char data)
{
  SPI.transfer(0x00);
  SPI.transfer(data);
}

void MusicPlayer::midiDemoPlayer(void)
{
  delay(1000);
  midiWriteData(0xB0, 0x07, 170);   //channel volume

  //GM2 Mode
  Serial.print("Fancy Midi Sounds\r\n");
  midiWriteData(0xB0, 0, 0x78);     //Bank select drums
  for(int instrument = 30 ; instrument < 31 ; instrument++)
  {
    Serial.print(" Instrument: ");
    Serial.println(instrument, DEC);

    midiWriteData(0xC0, instrument, 0);    //Set instrument number. 0xC0 is a 1 data byte command

    //Play notes from F#-0 (30) to F#-5 (90):
    for (int note = 27 ; note < 87 ; note++)
    {
      Serial.print("N:");
      Serial.println(note, DEC);
      //Note on channel 1 (0x90), some note value (note), middle velocity (0x45):
      midiNoteOn(0, note, 127);
      delay(50);

      //Turn off the note with a given off/release velocity
      midiNoteOff(0, note, 127);
      delay(50);
    }

    //delay 100ms between each instruments
    delay(100);
    }
}


