#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(9, 10);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  radio.openReadingPipe(1, 0xE8E8F0F0E1LL);
  radio.startListening();
}

void loop() {
  // put your main code here, to run repeatedly:
  char buf[100] = {0};
  if ( radio.available() ){
    radio.read( buf, 20 );
    Serial.print(buf);
  }
}
