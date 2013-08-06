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

#ifndef _EEPROM_H
#define _EEPROM_H

/**
 * EEPROM addresses
 */
#define EEPROM_FREQ_CHANNEL       0x00   // 1-byte register
#define EEPROM_NOT_USED           0x01   // 1-byte register
#define EEPROM_SYNC_WORD          0x02   // 2-byte register
#define EEPROM_DEVICE_ADDR        0x04   // 1-byte register
#define EEPROM_TX_INTERVAL        0x05   // 2-byte register

#define EEPROM_FIRST_CUSTOM       0x20

class EEPROM
{
  public:
    /**
     * readByte
     * 
     * Read byte from EEPROM
     *
     * @param address Address in EEPROM
     */
    unsigned char readByte(unsigned int address);

    /**
     * readInteger
     * 
     * Read unsigned integer from EEPROM
     *
     * @param address Address in EEPROM
     */
    unsigned int readInteger(unsigned int address);

    /**
     * readLong
     * 
     * Read unsigned long from EEPROM
     *
     * @param address Address in EEPROM
     */
    unsigned long readLong(unsigned int address);

    /**
     * writeByte
     * 
     * Write byte in EEPROM
     *
     * @param val byte to be written
     * @param address Address in EEPROM
     */
    void writeByte(unsigned char val, unsigned int address);

    /**
     * readSegment
     * 
     * Read buffer from EEPROM
     *
     * @param buffer array to be read
     * @param address Address in EEPROM
     * @pararm length Length to be read
     */
    void readSegment(unsigned char *buffer, unsigned int address, unsigned char length);

    /**
     * writeSegment
     * 
     * Write buffer in EEPROM
     *
     * @param buffer array to be written
     * @param address Address in EEPROM
     * @pararm length Length to be written
     */
    void writeSegment(unsigned char *buffer, unsigned int address, unsigned char length);

    /**
     * write
     *
     * Write integer or long value into EEPROM
     *
     * @param val Value to be written
     * @param addr Address in EEPROM
     * @param size amount of bytes to be written
     */
    template<class T> void writeInteger(T val, unsigned int addr, unsigned char size)
    {
      unsigned char i;

      for(i=0 ; i<size ; i++)
      {
        writeByte(val & 0xFF, addr + i);
        val >>= 8;
      }
    }
};

#endif

