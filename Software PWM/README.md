# Software PWM
Using the timer modules integrated into the MSP430 microcontrollers, outputs can be set with PWM functions using software or hardware. This example uses software to control the PWM outputs, specifically the led on pin 1.0 on each board used. Pressing the activation button changes the PWM for the output from 0% to 100% in incriments of 10% and starting at 50%. When the button is pressed and the PWM is at 100%, it resets the PWM back to 0%.

There is also debouncing code added in which, using the timers, prevents accidental bouncing of the button as to not input multiple presses of the button from a single press.

## BOARDS
MSP430G2553
	On this board, the button P1.3 is pressed in order to change the brightness of LED P1.0. LED P1.6 lights up when button P1.3 is pressed to indicate the button is being pressed.

MSP430FR6989
	On this board, the button P1.1 is pressed in order to change the brightness of LED P1.0. LED P9.7 lights up when button P1.1 is pressed to indicate the button is being pressed.
