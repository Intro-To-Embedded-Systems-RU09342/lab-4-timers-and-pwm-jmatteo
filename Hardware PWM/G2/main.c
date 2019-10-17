#include <msp430.h> 

// BY JACOB MATTEO 2019

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	P1DIR |= BIT0;              // SETS PIN 1.0 TO OUTPUT
	P1DIR |= BIT6;              // SETS PIN 1.6 TO OUTPUT
	P1SEL |= BIT6;              // SETS PWM ON PIN 1.0

	P1OUT  = BIT3;              // SETS BUTTON 1.3 TO INPUT
	P1REN |= BIT3;              // SETS PULL-UP RESISTOR ON P1.3
	P1IE  |= BIT3;              // INTERRUPT ON P1.3
	P1IES |= BIT3;              // BUTTON PRESS ACTIVATES INTERRUPT
	P1IFG  &= ~BIT3;             //SETS FLAG ON P1.3

	TA0CCR0 = 1000;             //SETS PERIOD TO 1kHz
	TA0CCTL1= OUTMOD_7;         //RESET TIMER
	CCR1 = 500;                 //CCR1 = 50% DUTY CYCLE
	TA0CCR1 = CCR1;             //SET PWM TO CCR1
	TA0CTL  = TASSEL_2 + MC_1;  //SETS TIMER TO SMCLK & UPMODE

	__bis_SR_register(LPM0_bits + GIE); //ENTERS LPM0 DURING INTERRUPT

}

#pragma vector=PORT1_VECTOR         //interrupt triggers on button
__interrupt void Button(void)
{
    P1OUT ^= BIT0;          //toggle LED

    if (CCR1 ==1000)        //CCR1 at Max?
    {
        CCR1 = 0;           //CCR1 = 0
    }
    else
    {
        CCR1 += 100;        //add 100 to CCR1
    }
    TA0CCR1 = CCR1;         //ser duty cycle to CCR1
    P1IFG = ~BIT3;          //turns off interrupt flag
}
