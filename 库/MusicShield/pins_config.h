/*
 * pins_config.h
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

#ifndef PINS_CONFIG_H
#define PINS_CONFIG_H

#include <avr/io.h>
#include <arduino.h>

////////////////////////for MMC////////////////////////////////////
#define SD_CS_PIN 10   //SD card select pin linked to pin10 of MCU

///////////////////////for vs10xx///////////////////////////////////
#define VS_XRESET A0
#define VS_DREQ   A1
#define VS_XDCS   A2
#define VS_XCS    A3

/////////////////////for keys//////////////////////////////////////
#define KEY_VD   3
#define KEY_BK   6
#define KEY_PS   5
#define KEY_NT   4
#define KEY_VU   7

////////////////////for LED///////////////////////////////////////
#define LED 	8

#endif

