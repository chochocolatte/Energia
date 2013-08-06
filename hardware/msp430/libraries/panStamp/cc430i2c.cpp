/**
 * Copyright (c) 2011 panStamp <contact@panstamp.com>
 * 
 * This file is part of the panStamp project.
 * 
 * panStamp  is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * any later version.
 * 
 * panStamp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with panStamp; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 
 * USA
 * 
 * Author: Daniel Berenguer
 * Creation date: 08/05/2013
 */

#include "cc430i2c.h"
#include "cc430pins.h"
#include "cc430x513x.h"

/**
 * init
 * 
 * Initialize I2C port
 *
 * @param address I2C slave address
 */
void CC430I2C::init(unsigned int address) 
{
//  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
   
  pinI2Cconfig();                           // Configure I2C pins

  UCB0CTL1 |= UCSWRST;                      // Enable SW reset
  UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
  UCB0CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK
  UCB0BR0 = 120;                            // fSCL = SMCLK/120 = ~100kHz
  UCB0BR1 = 0;
  UCB0I2CSA = address;                      // Slave Address
  UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation
  UCB0IFG = 0;                              // Reset interrupt flags
}

/**
 * send
 * 
 * Send data to I2C slave
 *
 * @param address I2C slave address
 * @param buf Buffer to be sent
 * @param len Amount of bytes to be transferred
 *
 * @return Amount of bytes transmitted
 */
unsigned int CC430I2C::send(unsigned int address, unsigned char *buf, unsigned int len) 
{
  unsigned int i, res = len;
  unsigned long timeout = I2C_TIMEOUT;

  init(address);                            // Initialize I2C port

  UCB0CTL1 |= UCTR + UCTXSTT;               // Send start condition and slave address

  while (!(UCB0IFG & UCTXIFG));

  // Send bytes
  for(i=0 ; i<len ; i++)
  {
    UCB0TXBUF = buf[i];
    while ((!(UCB0IFG & UCTXIFG)) && timeout--);
    if (!timeout)
    {
      res = 0;
      break;
    }
  }

  UCB0CTL1 |= UCTXSTP;                      // I2C stop condition
  
  // Ensure stop condition got sent
  while ((UCB0CTL1 & UCTXSTP));

  return res;
}

/**
 * receive
 * 
 * Receive data from I2C slave
 *
 * @param address I2C slave address
 * @param buf Buffer received
 * @param len Amount of bytes to be read
 *
 * @return Amount of bytes received
 */
unsigned int CC430I2C::receive(unsigned int address, unsigned char *buf, unsigned int len) 
{
  unsigned int i, res = len;
  unsigned long timeout = I2C_TIMEOUT;

  init(address);                            // Initialize I2C port

  UCB0CTL1 &= ~UCTR;                        // Receiver mode
  UCB0CTL1 |= UCTXSTT;                      // Send start condition

  // Poll for Start bit to complete
  while ((UCB0IFG & UCTXSTT));

  // For every byte
  for(i=0 ; i<len ; i++)
  {
    // Poll for UCRXIFG event to occur
    while ((!(UCB0IFG & UCRXIFG)) && timeout--);
    if (!timeout)
    {
      res = 0;
      break;
    }
    buf[i] = UCB0RXBUF;   // Read byte
  }

  UCB0CTL1 |= UCTXSTP;                    // I2C stop condition

  // Ensure stop condition got sent
  while ((UCB0CTL1 & UCTXSTP));

  return res;
}

