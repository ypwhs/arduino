/*
 * IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */

#include <IRremote.h>
IRrecv irrecv(11);
IRsend irsend;
decode_results results;

void setup()
{
  Serial.begin(9600);
  pinMode(3,OUTPUT);
  irrecv.enableIRIn(); // Start the receiver
  digitalWrite(3, HIGH);   // 点亮LED测试（需通过手机摄像模式观察）
  delay(300);             // 等待3秒
  digitalWrite(3, LOW);    // 结束测试
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    irrecv.resume();

  
    if(results.value == 0xC946B){
  irsend.sendNEC(0xC40BF,32);
    }
  }
}
