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

#ifndef _PANSTAMP_H
#define _PANSTAMP_H

#include "cc430core.h"
#include "cc430radio.h"
#include "cc430rtc.h"
#include "register.h"
#include "swpacket.h"
#include "config.h"
#include "repeater.h"

/**
 * RTC definitions
 */
#define RTC_250MS    0x03   // Timer 2 prescaler = 32
#define RTC_500MS    0x04   // Timer 2 prescaler = 64
#define RTC_1S       0x05   // Timer 2 prescaler = 128
#define RTC_2S       0x06   // Timer 2 prescaler = 256
#define RTC_8S       0x07   // Timer 2 prescaler = 1024

/**
 * Macros
 */
#define setSwapStatusCallBack(ptrFunc)   statusReceived = ptrFunc

#define eepromToFactoryDefaults()                             \
  EEPROM.write(EEPROM_SYNC_WORD, CCDEF_SYNC1);        \
  EEPROM.write(EEPROM_SYNC_WORD + 1, CCDEF_SYNC0);    \
  EEPROM.write(EEPROM_DEVICE_ADDR, CCDEF_ADDR);       \
  EEPROM.write(EEPROM_FREQ_CHANNEL, CCDEF_CHANNR);    \
  EEPROM.write(EEPROM_TX_INTERVAL, 0xFF);                     \
  EEPROM.write(EEPROM_TX_INTERVAL + 1, 0xFF)

#define setHighTxPower()    radio.setTxPowerAmp(PA_LongDistance)
#define setLowTxPower()     radio.setTxPowerAmp(PA_LowPower)

#define enableAntiPlayback()    security |= 0x01

#ifndef SWAP_EXTENDED_ADDRESS
#define swapAddress         radio.devAddress
#endif
#define swapNetworkId       radio.syncWord

/**
 * System states
 */
enum SYSTATE
{
  SYSTATE_RESTART = 0,
  SYSTATE_RXON,
  SYSTATE_RXOFF,
  SYSTATE_SYNC,
  SYSTATE_LOWBAT
};


/**
 * Class: PANSTAMP
 * 
 * Description:
 * panStamp main class
 */
class PANSTAMP
{
  private:
    /**
     * MCU core
     */
    CC430CORE core;
    /**
     * RTC module
     */
    CC430RTC rtc;

  public:
    /**
     * Pointer to repeater object
     */
    REPEATER *repeater;

    /**
     * CC430RADIO radio interface
     */
    CC430RADIO radio;
    
    #ifdef SWAP_EXTENDED_ADDRESS
    /**
     * SWAP extended address
     */
    SWADDR swapAddress;
    #endif

    /**
     * Security options
     */
    unsigned char security;

    /**
     * Security cyclic nonce
     */
    unsigned char nonce;
    
    /**
     * System state
     */
    unsigned char systemState;

    /**
     * Interval between periodic transmissions. 0 for asynchronous transmissions
     */
    unsigned int txInterval;

    /**
     * Smart encryption password
     */
    unsigned char encryptPwd[12];

    /**
     * enableRepeater
     *
     * Enable repeater mode
     */
    void enableRepeater(void);

    /**
     * enableRepeater
     *
     * Enable repeater mode
     *
     * @param maxHop Maximum repeater count. Zero if omitted
     */
    void enableRepeater(unsigned char maxHop=0);

    /**
     * SWAP status packet received. Callback function
     */
    void (*statusReceived)(SWPACKET *status);

    /**
     * PANSTAMP
     *
     * Class constructor
     */
    PANSTAMP(void);

    /**
     * init
     * 
     * Initialize panStamp board
     */
    void init(void);

    /**
     * reset
     * 
     * Reset panStamp
     */
    void reset(void);

    /**
     * wakeUp
     *
     * Wake from sleep mode
     *
     * @param rxOn Enter RX_ON state after waking up
     */
    void wakeUp(bool rxOn=true);

    /**
     * goToSleep
     *
     * put the MCU in sleep mode
     *
     * @param source Source of interruption (RTCSRC_VLO or RTCSRC_XT1)
     */
    void goToSleep(RTCSRC source=RTCSRC_XT1);

    /**
     * enterSystemState
     *
     * Enter system state
     *
     * @param state New system state
     */
    void enterSystemState(SYSTATE state);

    /**
     * setSmartPassword
     * 
     * Set Smart Encryption password
     * 
     * @param password Encryption password
     */
    void setSmartPassword(unsigned char* password);
};

/**
 * Global PANSTAMP object
 */
extern PANSTAMP panstamp;

/**
 * getRegister
 *
 * Return pointer to register with ID = regId
 *
 * @param regId Register ID
 */
REGISTER * getRegister(unsigned char regId);

#endif

