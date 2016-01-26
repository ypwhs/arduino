/*
 * MusicPlayer.h
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

#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <SD.h>
#include "Pins_config.h"
#include "vs10xx.h"
#include  <avr/pgmspace.h>

#define MaxVol 250
#define MinVol 100




/** Playing states definations. */
volatile typedef enum {
  PS_IDLE = 0,	   // Player idle						 
  PS_PLAY,         // Start to player                                    
  PS_PAUSE,        //Pause play                                         
  PS_RECORDING,    //Recording states                                   
} playingstatetype;

/** Control states definations. */
volatile typedef enum
{
  CS_EMPTY = 0,      // Have no control                                     
  CS_PLAYPAUSE,      // Play/pause button pressed                     
  CS_RECORDING,      // Play/pause button long pressed               
  CS_UP,             // Up button pressed                                                          
  CS_DOWN,           // Down button pressed                                                       
  CS_NEXT,           // Right button pressed                                                      
  CS_PREV,           // Left button pressed                                                       
} ctrlStatetype;

typedef struct songDesc
{
  char name[13];
  uint16_t index;
}songtype;



/************** key Internal*********************************/
volatile typedef enum
{
  KS_IDLE,KS_PRESS,
}keyState_t;

class key
{
public:
  void setKey(unsigned char io,unsigned char return_value);
  void scanKey(void);

private:
  unsigned char _io;
  unsigned char  _return_value;
  unsigned char _key_st;
  unsigned int  _keycnt;
};



/*****************class for the music player*******************/
class MusicPlayer
{
public:
  
  
  void begin(void);
  void play();
  void recording(char *recfile);

  //for Midi Player
  void beginMidi(void);
  void midiDemoPlayer(void);		
 
  

private:
  int Vol = 40;
  char songIndex;
  char MaxSong;
  
  void initIO(void);
  void initSD(void);
  void initTimer1();

  //for Midi Player
  void midiWriteData(char cmd, char high, char low);
  void midiNoteOn(char channel, char note, char rate);
  void midiNoteOff(char channel, char note, char rate);
  void midiSendByte(char data);
};



#endif
