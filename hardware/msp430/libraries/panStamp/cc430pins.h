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

#ifndef _CC430PINS_H
#define _CC430OINS_H

// UART
#define pinUARTmap()      P3MAP6 = PM_UCA0TXD; P3MAP7 = PM_UCA0RXD
#define pinUARTconfig()   P3SEL |= BIT6 | BIT7;     // Set P3.6 and P3.7 to USCI Mode
// I2C
#define pinI2Cmap()       P1MAP6 = PM_UCB0SCL; P1MAP7 = PM_UCB0SDA
#define pinI2Cconfig()    P1SEL |= BIT6 | BIT7;     // Set P1.6 and P1.7 to USCI function

#endif

