#include <msp430.h> 

//By Alex Jackson, edited by Jacob Matteo

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               //stop watchdog timer

        P1DIR |=  BIT0;                     //set 1.0 to output
        P1OUT &= ~BIT0;
        P1DIR |=  BIT6;                     //set 1.6 to output
        P1OUT &= ~BIT6;
        P1DIR &= ~BIT3;                     //set 1.3 to input
        P1IFG  =   0;
        P1IES |=  BIT3;                     //button press triggers interrupts
        P1IE  |=  BIT3;

        //debounce timer
        TA0CTL   |= TASSEL_2;               //SMCLK
        TA0CTL   |= MC_2;                   //continuous mode
        TA0CCR0   = 65;                     //counts to 2ms
        TA0CCTL0 |= CCIE;
        TA0CTL   |= TAIE;                   //interrupts

        //PWM timer
        TA1CTL   |= TASSEL_2;               //SMCLK
        TA1CTL   |= MC_1;                   //up mode
        TA1CCR0   = 999;                    //1000 Hz (highest value for PWM)
        TA1CCR1   = 499;                    //start LED at 500 Hz (50%)
        TA1CCTL0 |= CCIE;
        TA1CCTL1 |= CCIE;
        TA1CTL   |= TAIE;                   //interrupts

        __bis_SR_register(LPM0_bits + GIE); //LPM0

        while(1) {
                __no_operation();
            }
    }

    void __attribute__ ((interrupt(PORT1_VECTOR))) Port_1 (void)
    { //debounce
        P1IFG  &= ~BIT3;
        TA0R    =  0;                       //reset timer
        TA0CTL &= ~MC_3;                    //start timer in continuous mode
        TA0CTL |=  MC_2;
    }

    void __attribute__((interrupt(TIMER0_A0_VECTOR))) timer0_A0 (void)
    {//PWM
        TA0CTL &= ~TAIFG;                   //clear interrupt flag
        TA0CTL &= ~MC_3;                    //stop debounce timer

        //if statement for button press
        if(!(P1IN & BIT3) && (P1IES & BIT3))
        {
            P1IES &= ~BIT3;                 //button interrupt
            P1OUT |=  BIT6;                 //turn on LED

            if(TA1CCR1 < 901)              //if duty cycle is less than 901, increase by 100
            {
                TA1CCR1 += 100;
                TA1CTL  |= MC_1;
            }
            else                            //else, stop timer and set to 0
            {
                TA1CCR1 = 0;
                P1OUT  &= ~BIT0;
                TA1CTL &= ~MC_3;
            }
        }

        //if statement for button release
        if((P1IN & BIT3) && !(P1IES & BIT3))
        {
            P1IES |=  BIT3;
            P1OUT &= ~BIT6;                 //turn off led 1.6
        }
    }

    void __attribute__((interrupt(TIMER1_A0_VECTOR))) timer1_A0 (void)
    {//ISR timer 1 A0
        TA1CTL &= ~TAIFG;                   //clear interrupt flag
        P1OUT  |= BIT0;                     //turn on led 1.0
    }

    void __attribute__((interrupt(TIMER1_A1_VECTOR))) timer1_A1 (void)
    {//ISR timer 1 A1
        if(TA1IV == 0x02)                   //if interrupt was generated from CCR1 compare
            P1OUT &= ~BIT0;                 //turn off led 1.0
    }
