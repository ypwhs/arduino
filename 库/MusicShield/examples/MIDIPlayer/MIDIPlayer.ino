/*
 midiDemoPlayer
 This example shows how to play MIDI.
 */

#include <SPI.h>
#include <SD.h>
#include <MusicPlayer.h>

MusicPlayer player;

void setup()
{
  Serial.begin(115200);
  player.beginMidi();
}

void loop()
{
  player.midiDemoPlayer();
}
