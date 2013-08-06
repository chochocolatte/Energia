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

#include "panstamp.h"
#include "commonregs.h"


DEFINE_COMMON_REGINDEX_START()
DEFINE_COMMON_REGINDEX_END()

/**
 * Array of registers
 */
extern REGISTER* regTable[];
extern unsigned char regTableSize;

/**
 * radioISR
 *
 * Radio interrupt routine
 */
#pragma vector=CC1101_VECTOR
__interrupt void radioISR(void)
{
  static CCPACKET ccPacket;
  unsigned int coreIntSource = RF1AIV;            // Radio Core      interrupt register

  // Radio Core interrupt
  if(coreIntSource)
  {
    // Check for SYNC interrupt
    if(coreIntSource == RF1AIV_RFIFG9)
    {
      if(MRFI_SYNC_PIN_INT_IS_ENABLED())
      {
        static CCPACKET ccPacket;

        /*  clear the sync pin interrupt, run sync pin ISR */
        /*
         *  NOTE!  The following macro clears the interrupt flag but it also *must*
         *  reset the interrupt capture.  In other words, if a second interrupt
         *  occurs after the flag is cleared it must be processed, i.e. this interrupt
         *  exits then immediately starts again.  Most microcontrollers handle this
         *  naturally but it must be verified for every target.
         */

        MRFI_CLEAR_SYNC_PIN_INT_FLAG();
        MRFI_DISABLE_SYNC_PIN_INT();

        // Any packet waiting to be read?
        if (panstamp.radio.receiveData(&ccPacket) > 0)
        {
          // Is CRC OK?
          if (ccPacket.crc_ok)
          {
            static SWPACKET swPacket;
            REGISTER *reg;
            static bool eval = true;

            swPacket = SWPACKET(ccPacket);

            // Repeater enabled?
            if (panstamp.repeater != NULL)
              panstamp.repeater->packetHandler(&swPacket);

            // Smart encryption locally enabled?
            if (panstamp.security & 0x02)
            {
              // OK, then incoming packets must be encrypted too
              if (!(swPacket.security & 0x02))
                eval = false;
            }

            if (eval)
            {
              // Function
              switch(swPacket.function)
              {
                case SWAPFUNCT_CMD:
                  // Command not addressed to us?
                  if (swPacket.destAddr != panstamp.radio.devAddress)
                    break;
                  // Current version does not support data recording mode
                  // so destination address and register address must be the same
                  if (swPacket.destAddr != swPacket.regAddr)
                    break;
                  // Valid register?
                  if ((reg = getRegister(swPacket.regId)) == NULL)
                    break;
                  // Anti-playback security enabled?
                  if (panstamp.security & 0x01)
                  {
                    // Check received nonce
                    if (panstamp.nonce != swPacket.nonce)
                    {
                      // Nonce missmatch. Transmit correct nonce.
                      reg = getRegister(REGI_SECUNONCE);
                      reg->sendSwapStatus();
                      break;
                    }
                  }
                  // Filter incorrect data lengths
                  if (swPacket.value.length == reg->length)
                    reg->setData(swPacket.value.data);
                  else
                    reg->sendSwapStatus();
                  break;
                case SWAPFUNCT_QRY:
                  // Only Product Code can be broadcasted
                  if (swPacket.destAddr == SWAP_BCAST_ADDR)
                  {
                    if (swPacket.regId != REGI_PRODUCTCODE)
                      break;
                  }
                  // Query not addressed to us?
                  else if (swPacket.destAddr != panstamp.radio.devAddress)
                    break;
                  // Current version does not support data recording mode
                  // so destination address and register address must be the same
                  if (swPacket.destAddr != swPacket.regAddr)
                    break;
                  // Valid register?
                  if ((reg = getRegister(swPacket.regId)) == NULL)
                    break;
                  reg->getData();
                  break;
                case SWAPFUNCT_STA:
                  // User callback function declared?
                  if (panstamp.statusReceived != NULL)
                    panstamp.statusReceived(&swPacket);
                  break;
                default:
                  break;
              }
            }
          }
        }
        MRFI_ENABLE_SYNC_PIN_INT();
      }
    }
  }
}

/**
 * PANSTAMP
 *
 * Class constructor
 */
PANSTAMP::PANSTAMP(void)
{
  statusReceived = NULL;
  repeater = NULL;
}

/**
 * enableRepeater
 *
 * Enable repeater mode
 *
 * @param maxHop Maximum repeater count. Zero if omitted
 */
void PANSTAMP::enableRepeater(unsigned char maxHop)
{
  if (repeater == NULL)
  {
    static REPEATER repe;
    repeater = &repe;
    repeater->init(maxHop);
  }

  if (maxHop == 0)
    repeater->stop();
}

/**
 * getRegister
 *
 * Return pointer to register with ID = regId
 *
 * @param regId Register ID
 */
REGISTER * getRegister(unsigned char regId)
{
  if (regId >= regTableSize)
    return NULL;

  return regTable[regId]; 
}

/**
 * init
 * 
 * Initialize panStamp board
 */
void PANSTAMP::init() 
{
  unsigned char i;

  // Initialize MCU core
  core.init();

  // Intialize registers
  for(i=0 ; i<regTableSize ; i++)
    regTable[i]->init();

  // Setup radio interface
  radio.init();

  // Security disabled by default
  security = 0;

  delayMicroseconds(50);  

  // Default values
  nonce = 0;
}

/**
 * reset
 * 
 * Reset panStamp
 */
void PANSTAMP::reset() 
{
  // Tell the network that our panStamp is restarting
  systemState = SYSTATE_RESTART;
  getRegister(REGI_SYSSTATE)->sendSwapStatus();

  // Reset panStamp
  #ifdef PANSTAMP_AVR
  wdt_disable();  
  wdt_enable(WDTO_15MS);
  while (1) {}
  #endif
}

/**
 * wakeUp
 *
 * Wake from sleep mode
 *
 * @param rxOn Enter RX_ON state after waking up
 */
void PANSTAMP::wakeUp(bool rxOn) 
{
  radio.wakeUp();
}

/**
 * goToSleep
 *
 * put the MCU in sleep mode
 *
 * @param source Source of interruption (RTCSRC_VLO or RTCSRC_XT1)
 */
void PANSTAMP::goToSleep(RTCSRC source)
{
  // Sleep
  rtc.sleep(txInterval, source);
}

/**
 * enterSystemState
 *
 * Enter system state
 *
 * @param state New system state
 */
void PANSTAMP::enterSystemState(SYSTATE state)
{
  // Enter SYNC mode (full Rx mode)
  unsigned char newState[] = {state};
  regTable[REGI_SYSSTATE]->setData(newState);
}

/**
 * setSmartPassword
 * 
 * Set Smart Encryption password
 * 
 * @param password Encryption password
 */
void PANSTAMP::setSmartPassword(unsigned char* password)
{
  // Save password
  memcpy(encryptPwd, password, sizeof(encryptPwd));
  // Enable Smart Encryption
  security |= 0x02;
}

/**
 * Pre-instantiate PANSTAMP object
 */
PANSTAMP panstamp;

