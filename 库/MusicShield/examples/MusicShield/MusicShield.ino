/*
  Music Shield

 This example shows how to play music with music shield.
 The circuit:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 10
 ** XCS - A3 
 ** XDCS - A2
 ** DREQ - A1
 ** XRESET - A0
 ** LED - pin 8
 ** SDDetect - pin 9
 
 This example code is in the public domain.
 */

#include <SPI.h>
#include <SD.h>
#include <MusicPlayer.h>

MusicPlayer player;

void setup()
{
  Serial.begin(115200);
  player.begin();
}

void loop()
{
  player.play();
}


