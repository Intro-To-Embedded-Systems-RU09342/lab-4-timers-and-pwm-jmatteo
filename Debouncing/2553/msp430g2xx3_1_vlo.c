/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************
 * 
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//******************************************************************************
//  MSP430G2xx3 Demo - Software Toggle P1.0, MCLK = VLO/8
//
//  Description; Pulse P1.0 with a 1/100 active duty cycle using software.
//  Ultra-low frequency ~ 1.5kHz, ultra-low power active mode demonstrated.
//  ACLK = VL0, MCLK = VLO/8 ~1.5kHz, SMCLK = n/a
//
//                MSP430G2xx3
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P1.0|-->LED
//
//  D. Dang
//  Texas Instruments, Inc
//  December 2010
//   Built with CCS Version: 4.2.0 and IAR Embedded Workbench Version: 5.10
//
//  EDITED BY JACOB MATTEO 2019
//******************************************************************************

#include <msp430.h>

int main(void)
{
  WDTCTL   =  WDTPW + WDTHOLD;              // Stop watchdog timer
  BCSCTL3 |=  LFXT1S_2;                     // LFXT1 = VLO
  P1SEL    =  0;                            //port 1 is i/o
  P1DIR   |=  BIT0;                         //set p1.0 to output (to LED)
  P1DIR   &= ~BIT3;                         //set p1.3 to input (from switch)
  P1IE    |=  BIT3;                         //enable interrupts on p1.3
  P1IES   |=  BIT3;                         //set interrupt to flag on a falling edge

  //initialize count up to value
  TACCR0 = 0x0041;                          //2ms delay time

  __bis_SR_register(GIE);                   //enable interrupts

  while(1);
  return;
}

//interrupt service routine
#pragma vector = PORT1_VECTOR
__interrupt void PORT1_vector(void)//
{
     //check button press and release triggers
     if(P1IES & BIT3)                       //falling edge trigger (button press)
     {
         //disable interrupts and activate timer
         P1IE &= ~BIT3;                     //disables interrupts on port 1.3
         TA0CTL = TASSEL_1|MC_1|TACLR;      //sets up the timer to the 32 kHz clock in up mode

         while(TA0R != TACCR0);             //counts 2ms

         TA0CTL =  MC_0|TACLR;              //turns the timer off
         P1OUT ^=  BIT0;                    //changes LED state
         P1IE  |=  BIT3;                    //enables interrupts on port 1.3
         P1IES &= ~BIT3;                    //set interrupt to flag on a rising edge
     }

     else // rising edge trigger (button release)
     {
         //disable interrupts and activate timer
         P1IE   &= ~BIT3;                   //disables interrupts on port 1.3
         TA0CTL =   TASSEL_1|MC_1|TACLR;    //sets up the timer to the 32 kHz clock in up mode

         while(TA0R != TACCR0);             //counts 2ms
         TA0CTL =  MC_0|TACLR;              //turns the timer off
         P1IE  |=  BIT3;                    //enables interrupts on port 1.3
         P1IES |=  BIT3;                    //set interrupt to flag on a rising edge
     }
     P1IFG &= ~BIT3; //turn off the flag
}
