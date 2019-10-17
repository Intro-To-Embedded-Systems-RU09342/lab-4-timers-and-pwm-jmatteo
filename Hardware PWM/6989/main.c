#include <msp430.h> 


// By JACOB MATTEO 2019

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;       // disables the GPIO power on high-impedance mode

	P1DIR  |=  BIT0;             //SETS P1.0 TO OUTPUT
	P1SEL0 |=  BIT0;             //SETS P1.0 TO PWM
	P9DIR  |=  BIT7;             //SETS P9.7 TO OUTPUT
	P9OUT  &= ~BIT7;             //SETS P9.7 TO OFF

	P1DIR  &= ~BIT1;             //SETS P1.1 TO INPUT
	P1REN  |=  BIT1;             //ENABLES P1.1 RESISTOR
	P1OUT  |=  BIT1;             //SETS RESISTOR TO PULL-UP
	P1IFG   =  0x00;             //CLEARS INTERRUPT FLAGS
	P1IES  |=  BIT1;             //SETS INTERRUPT TO BUTTON PRESS
	P1IE   |=  BIT1;             //ENABLES INTERRUPT ON BUTTON

    TA1CTL   |= TASSEL_2;        //SETS TIMER A1 TO USE SMCLK
    TA1CTL   |= MC_2;            //SETS CONTINUOUS MODE
    TA1CCR0   = 65;              //SETS DEBOUNCE TIME TO 2ms
    TA1CCTL0 |= CCIE;            //SET INTERRUPT TO OFF
    TA1CTL   |= TAIE;            //SETS TIMER INTERRUPTS TO OFF

	TA0CTL   |= TASSEL_2;        //SETS TIMER A0 TO USE SMCLK
	TA0CTL   |= MC_2;            //SETS CONTINUOUS MODE
	TA0CCR0   = 999;             //SETS PWM TO 1000 Hz (100%)
	TA0CCR1   = 500;             //SETS PWM TO 500Hz (50%)
	TA0CCTL1 |= OUTMOD_7;        //RESETS OUTPUT AT CCR0, SERS AT CCR1

	__bis_SR_register(LPM0_bits + GIE);    //enter LPM0 and enables interrupts

	while(1)                     //loops forever
	{
	    __no_operation();
	}
}

void __attribute__ ((interrupt(PORT1_VECTOR))) Port_1 (void) //debouncing
        {
            P1IFG  &= ~BIT1;        // CLEAR INTERRUPT FLAG
            TA1R    =  0;           // RESET TIMER
            TA1CTL &= ~MC_3;        // START TIMER IN CONTINUOUS MODE
            TA1CTL |=  MC_2;
        }

void __attribute__((interrupt(TIMER1_A0_VECTOR))) timer1_A0 (void) //PWM
        {
            TA1CTL &= ~TAIFG;       //CLEARS INTERRUPT FLAGS
            TA1CTL &= ~MC_3;        //STOP DEBOUNCE TIMER

            if(!(P1IN & BIT1) && (P1IES & BIT1)) //IF BUTTON IS PRESSED
            {
                P1IES &= ~BIT1;     //MAKES INTERRUPT GENERATE ON NEXT EDGE
                P9OUT |=  BIT7;     //TURNS ON SECOND LED (P9.7)

                if(TA0CCR1 <= 900)   //if the PWM is <= 90%
                {
                    TA0CCR1  += 100;        //INCREASE THE PWM BY 100 (10%)
                    TA0CTL   |= MC_1;        //UP MODE
                    TA0CCTL1 |= OUTMOD_7;
                }
                else
                {
                    TA0CCR1   =  0;         //SET PWM TO 0%
                    TA0CTL   &= ~MC_3;       //STOP TIMER
                    TA0CCTL1 &= ~OUTMOD_7;  //TURN OUTPUT OFF
                    TA0CCTL1 &= ~OUT;
                }
            }

            if((P1IN & BIT1) && !(P1IES & BIT1)) //ON BUTTON RELEASE
            {
                P1IES |=  BIT1;        //CHANGE BUTTON INTERRUPT EDGE SLOPE
                P9OUT &= ~BIT7;        //TURN OFF P9.7
            }
        }
