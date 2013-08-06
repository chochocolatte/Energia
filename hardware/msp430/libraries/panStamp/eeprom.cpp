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
 * Creation date: 07/17/2013
 */

#include "eeprom.h"

/**
 * readByte
 * 
 * Read byte from EEPROM
 *
 * @param address Address in EEPROM
 */
unsigned char EEPROM::readByte(unsigned int address) 
{
  unsigned char res = 0;

  return res;
}

/**
 * readInteger
 * 
 * Read unsigned integer from EEPROM
 *
 * @param address Address in EEPROM
 */
unsigned int EEPROM::readInteger(unsigned int address) 
{
  unsigned int res = 0;

  return res;
}

/**
 * readLong
 * 
 * Read unsigned long from EEPROM
 *
 * @param address Address in EEPROM
 */
unsigned long EEPROM::readLong(unsigned int address) 
{
  unsigned long res = 0;

  return res;
}

/**
 * readLong
 * 
 * Read unsigned long from EEPROM
 *
 * @param address Address in EEPROM
 */
void EEPROM::readSegment(unsigned char *buffer, unsigned int address, unsigned char length) 
{
  //
}

/**
 * writeByte
 * 
 * Write byte in EEPROM
 *
 * @param val byte to be written
 * @param address Address in EEPROM
 */
void EEPROM::writeByte(unsigned char val, unsigned int address) 
{
  //
}

/**
 * writeSegment
 * 
 * Write buffer in EEPROM
 *
 * @param buffer array to be written
 * @param address Address in EEPROM
 * @pararm length Length to be written
 */
void EEPROM::writeSegment(unsigned char *buffer, unsigned int address, unsigned char length) 
{
  //
}

