/*
  I2C.cpp

  Copyright (C) 2013 Jan Rychter
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
  
  Modified 2015 by Mike Lu (NiHaoMike) for OpenDAC
*/
#include "I2C.h"
#include <Energia.h>
static volatile uint8_t done;
void I2CWait() {
  delayMicroseconds(3);
  for(unsigned int k=0; k<60000; k++) { //~120ms timeout
    if(done)
      break;
    delayMicroseconds(2);
  }
  done=0;
}
void I2CStart() {
  USISRL = 0x00;
  USICTL0 |= (USIGE|USIOE);
  USICTL0 &= ~USIGE;
  delayMicroseconds(4);
}
void I2CRestart() {
  USICTL0 |= USIOE;
  USISRL = 0xff;
  USICNT = (USICNT & 0xE0) | 1;
  I2CWait();
  I2CStart();
}
void I2CStop() {
  USICTL0 |= USIOE;
  USISRL = 0x00;
  USICNT |=  0x01;
  I2CWait();
  USISRL = 0x0FF;
  USICTL0 |= USIGE;
  USICTL0 &= ~(USIGE+USIOE);
  delayMicroseconds(4);
}
uint8_t I2CWrite(uint8_t dat) {
  USICTL0 |= USIOE;
  USISRL = dat;
  USICNT = (USICNT & 0xE0) | 8;
  I2CWait();
  USICTL0 &= ~USIOE;
  USICNT |= 0x01;
  I2CWait();
  return USISRL;
}
uint8_t I2CRead(uint8_t ack) {
  uint8_t dat;
  USICTL0 &= ~USIOE;
  USICNT = (USICNT & 0xE0) | 8;
  I2CWait();
  dat=USISRL;
  USICTL0 |= USIOE;
  if(ack)
    USISRL = 0x00;
  else
    USISRL = 0xff;
  USICNT |= 0x01;
  return dat;
}
#pragma vector = USI_VECTOR
__interrupt void USI_TXRX(void)
{
  if(USICTL1 & USIIFG)
    done=1;
  USICTL1 &= ~USIIFG;
}

void i2c_init() {
  noInterrupts();
  done=0;
  USICTL0 = USIPE6+USIPE7+USIMST+USISWRST;  // Port & USI mode setup
  USICTL1 = USII2C+USIIE;                   // Enable I2C mode & USI interrupt
  USICKCTL = USIDIV_7|USISSEL_2 | USICKPL;
  USICNT |= USIIFGCC;                       // Disable automatic clear control
  USICTL0 &= ~USISWRST;                     // Enable USI
  USICTL1 &= ~USIIFG;                       // Clear pending flag
  interrupts();
}
