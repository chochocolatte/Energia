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
 * Creation date: 06/21/2013
 */

#include "cc430rtc.h"
#include "cc430x513x.h"
#include "Energia.h"

/**
 * Macros
 */
/**
 * RTC_D TIMER
 */
#define RTC_SET_ACLK_XT1()                 UCSCTL4 = SELA__XT1CLK | SELS__DCOCLKDIV | SELM__DCOCLKDIV
#define RTC_SET_ACLK_VLO()                 UCSCTL4 = SELA__VLOCLK | SELS__DCOCLKDIV | SELM__DCOCLKDIV
#define RTC_RESET_COUNTER()                RTCNT1=0; RTCNT2=0; RTCNT3=0; RTCNT4=0
#define RTC_START_8BIT_COUNTER()           (RTCCTL1 = 0x00)
#define RTC_START_16BIT_COUNTER()          (RTCCTL1 = 0x01)
#define RTC_START_24BIT_COUNTER()          (RTCCTL1 = 0x02)
#define RTC_START_32BIT_COUNTER()          (RTCCTL1 = 0x03)
#define RTC_STOP_COUNTER()                 (RTCCTL1 = 0x40)
#define RTC_SET_TICKS(ticks)               \
  RTCNT4 = (ticks >> 24) & 0xFF;           \
  RTCNT3 = (ticks >> 16) & 0xFF;           \
  RTCNT2 = (ticks >> 8) & 0xFF;            \
  RTCNT1 = ticks & 0xFF;

#define RTC_ISR_ENABLE()                   (RTCCTL0 |= RTCTEVIE)
#define RTC_ACK_ISR()                      (RTCCTL0 &= ~RTCTEVIFG)
// 32.768 KHz cycles for 1 second
#define RTC_32K_CYCLES_1SEC                32768UL
// VLO cycles for 1 second
#define RTC_VLO_CYCLES_1SEC                10000UL


/**
 * rtcISR
 * 
 * RTC ISR function
 */
unsigned char status = LOW;
#pragma vector=RTC_VECTOR
__interrupt void rtcISR(void)
{
  RTC_ACK_ISR();
  __bic_SR_register(LPM3_bits);  // clears the bits corresponding to LPM3 and exits the low power mode
  RTC_STOP_COUNTER();
  RTC_RESET_COUNTER();

//digitalWrite(13, LOW);
}

/**
 * sleep
 * 
 * Put panStamp into Power-down state during "time".
 * This function uses RTC connected to an external 32.768KHz crystal
 * in order to exit (interrupt) from the power-down state
 * 
 * @param time Sleeping time in seconds
 * @param ACLK source (RTCSRC_XT1 or RTCSRC_VLO)
 */
void CC430RTC::sleep(unsigned int time, RTCSRC source) 
{
  if (time == 0)
    return;

  unsigned long ticks = 0xFFFFFFFF;

  switch(source)
  {
    case RTCSRC_XT1:
      RTC_SET_ACLK_XT1();             // Connect ACLK to 32.768 KHz crystal
      ticks -= (RTC_32K_CYCLES_1SEC * time);
      break;
    case RTCSRC_VLO:
      RTC_SET_ACLK_VLO();             // Connect ACLK to VLO (Internal 10KHz oscillator)
      ticks -= (RTC_VLO_CYCLES_1SEC * time);
      break;
    default:
      break;
  }

  RTC_SET_TICKS(ticks);               // Initialize 32-bit counter
  RTC_ISR_ENABLE();                   // Enable RTC interrupt
  RTC_START_32BIT_COUNTER();          // Start RTC counter with 32-bit overflow

  __bis_SR_register(LPM3_bits + GIE); // Enter LPM3 with interrupts
 __no_operation();

//digitalWrite(13, HIGH);
}

