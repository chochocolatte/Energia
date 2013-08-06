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

#ifndef _CC430I2C_H
#define _CC430I2C_H

#define I2C_TIMEOUT     0xFF

class CC430I2C
{
  private:
    /**
     * init
     * 
     * Initialize I2C port
     *
     * @param address I2C slave address
     */
    void init(unsigned int address);

  public:
    /**
     * send
     * 
     * Send data to I2C slave
     *
     * @param address I2C slave address
     * @param buf Array of bytes to be sent
     * @param len Amount of bytes to be transferred
     *
     * @return Amount of bytes transmitted
     */
    unsigned int send(unsigned int address, unsigned char *buf, unsigned int len);

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
    unsigned int receive(unsigned int address, unsigned char *buf, unsigned int len);
};

#endif

