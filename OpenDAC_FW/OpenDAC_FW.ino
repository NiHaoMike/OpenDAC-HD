/*
 * OpenDAC_FW.ino - MSP430 firmware for OpenDAC
 * 
 * Design inspired by Diana Navarro.
 * 
 * Copyright (C) 2015 Mike Lu.
 * 
 * This file is part of OpenDAC.
 *
 * OpenDAC is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenDAC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenDAC.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "I2C.h"
volatile unsigned char flags; //used to pass events from ISRs to main loop
unsigned char volume;
unsigned char temp; //just a general purpose temporary register
volatile unsigned long deglitch; //variable used to prevent relay chattering
void setup() {
  digitalWrite(6,LOW);
  pinMode(6,OUTPUT);
  digitalWrite(4,LOW);
  pinMode(4,OUTPUT);
  digitalWrite(7,LOW);
  pinMode(7,OUTPUT);
  flags=0x08;
  P2DIR=0x07;
  P2REN=0x38;
  P2OUT=0x39;
  i2c_init();
  CACTL1=0x08;
  CACTL2=0x0E;
  CAPD=0x03;
  volume=0xF7;
  deglitch=millis();
  attachInterrupt(11,QEPint,FALLING);
  attachInterrupt(13,Pushint,FALLING);
  digitalWrite(5,HIGH);
  pinMode(5,OUTPUT);
  delay(10);
  if(P2IN&0x40) {
    P2OUT=0x78;
    flags=0;
    while((flags&0x07)==0)
      sleep(100);
    flags=0x08;
  }
  P1SEL=0xC8;
  P1SEL2=0x08;
  P2DIR=0x47;
  P2OUT=0x3A;
  initSys();
  P2OUT=0x3C;
  delay(250);
  attachInterrupt(18,DIXint,CHANGE);
}
void loop() {
  while(flags&0x01) {
    flags&=0xFE;
    temp=P2OUT;
    if(volume<255) {
      P2OUT=0x38;
      volume++;
    }
    else
      P2OUT=0x39;
    updateVolume();
    delay(50);
    P2OUT=temp;
  }
  while(flags&0x02) {
    flags&=0xFD;
    temp=P2OUT;
    if(volume>14) {
      P2OUT=0x38;
      volume--;
    }
    else
      P2OUT=0x39;
    updateVolume();
    delay(50);
    P2OUT=temp;
  }
  if(flags&0x04) {
    P2OUT=0x78;
    digitalWrite(7,LOW);
    sleep(100);
    digitalWrite(6,LOW);
    digitalWrite(4,LOW);
    flags=0;
    while((flags&0x07)==0)
      sleep(100);
    P2OUT=0x3B;
    initSys();
    delay(200);
    flags=0x08;
  }
  if((P2IN&0x80)==0) {
    if((digitalRead(7)==0)&&((millis()-deglitch)>250)) {
      deglitch=millis();
      digitalWrite(7,HIGH);
    }
    if(flags&0x08) {
      I2CStart();
      I2CWrite(0x82);
      I2CWrite(0x39);
      I2CRestart();
      I2CWrite(0x83);
      temp=I2CRead(0);
      I2CStop();
      switch(temp) {
        case 0x0: //invalid
          break;
        case 0x8: //44.1kHz
          P2OUT=0x3A;
          flags&=0xF7;
          break;
        case 0x9: //48kHz
          P2OUT=0x3E;
          flags&=0xF7;
          break;
        case 0xA: //64kHz
        case 0xB: //88.2kHz
          P2OUT=0x3C;
          flags&=0xF7;
          break;
        case 0xC: //96kHz
          P2OUT=0x3D;
          flags&=0xF7;
          break;
        case 0xD: //128kHz
        case 0xE: //176.4kHz
        case 0xF: //192kHz
          P2OUT=0x3F;
          flags&=0xF7;
          break;
        default:
          P2OUT=0x39;
          break;
      }
      sleep(250);
    }
  }
  else {
    if((digitalRead(7)==1)&&((millis()-deglitch)>500)) {
      deglitch=millis();
      digitalWrite(7,LOW);
    }
    P2OUT=0x3B;
    delay(150);
  }
}
void QEPint() {
  if(P2IN&0x10)
    flags|=0x02;
  else
    flags|=0x01;
}
void Pushint() {
  flags|=0x04;
}
void DIXint() {
  flags|=0x08;
  if((P2IN&0x80)&&((millis()-deglitch)>500)) {
    deglitch=millis();
    digitalWrite(7,LOW);
  }
}
void initSys() {
  digitalWrite(6,LOW);
  pinMode(6,OUTPUT);
  digitalWrite(4,LOW);
  pinMode(4,OUTPUT);
  delay(5);
  digitalWrite(4,HIGH);
  delay(100);
  initDAC();
  I2CStart();
  I2CWrite(0x82);
  I2CWrite(0x23);
  I2CWrite(0x05); //release error after 12 samples
  I2CWrite(0x00);
  I2CWrite(0x09); //flag error on non PCM or PLL unlock
  I2CWrite(0x09); //only accept PCM input
  I2CStop();
  I2CStart();
  I2CWrite(0x82);
  I2CWrite(0x30);
  I2CWrite(0x12); //enable automatic clock ratio selection
  I2CStop();
  I2CStart();
  I2CWrite(0x82);
  I2CWrite(0x34);
  I2CWrite(0x40); //enable coax input amplifier, set input to RXIN0
  I2CStop();
}
void initDAC() {
  digitalWrite(6,LOW);
  pinMode(6,OUTPUT);
  delay(5);
  digitalWrite(6,HIGH);
  delay(100);
  I2CStart();
  I2CWrite(0x9e);
  I2CWrite(16);
  I2CWrite(volume);
  I2CWrite(volume);
  I2CWrite(0xD0);
  I2CStop();
}
void updateVolume() {
  I2CStart();
  I2CWrite(0x9e);
  I2CWrite(16);
  I2CWrite(volume);
  I2CWrite(volume);
  I2CStop();
}
