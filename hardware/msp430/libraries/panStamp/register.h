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
 * Creation date: 06/03/2013
 */

#ifndef _REGISTER_H
#define _REGISTER_H

#include "Arduino.h"

extern unsigned char regIndex;

/**
 * Class: REGISTER
 * 
 * Description:
 * Register class
 */
class REGISTER
{
  private:
    /**
     * Pointer to the register "updater" function
     *
     *  @param rId Register ID     
     */
    const void (*updateValue)(unsigned char rId);

    /**
     * Pointer to the register "setter" function
     *
     *  @param rId Register ID     
     *  @param v New register value
     */
    const void (*setValue)(unsigned char rId, unsigned char *v);

  public:
    /**
     * Register id
     */
    const unsigned char id;
    
    /**
     * Register value
     */
    unsigned char *value;
    
    /**
     * Data length
     */
    const unsigned char length;

    /**
     * REGISTER
     * 
     * Constructor
     * 
     * @param val Pointer to the register value
     * @param len Length of the register value
     * @param getValH Pointer to the getter function
     * @param setValH Pointer to the setter function
     */
    REGISTER(unsigned char *val, const unsigned char len, const void (*updateValH)(unsigned char rId), const void (*setValH)(unsigned char rId, unsigned char *v)):id(regIndex++), value(val), length(len), updateValue(updateValH), setValue(setValH) {};

    /**
     * getData
     * 
     * Update and get register value
     * 
     */
    void getData();

    /**
     * setData
     * 
     * Set register value
     * 
     * 'data'	New register value
     */
    void setData(unsigned char *data);

    /**
     * sendSwapStatus
     * 
     * Send SWAP status message
     */
    void sendSwapStatus(void);

    /**
     * setRegValue
     *
     * Set register value from different data formats
     * Use this method to simplify LE to BE conversion
     *
     * @param val New register value
     */
    template<class T> void setRegValue(T val)
    {
      uint8_t i;

      for(i=0 ; i<length ; ++i)
      {
        value[i] = val & 0xFF;
        val >>= 8;
      }
    }
};

/**
 * Array of registers
 */
extern REGISTER* regTable[];

/**
 * Extern global functions
 */
extern void setupRegisters(void);

#endif

