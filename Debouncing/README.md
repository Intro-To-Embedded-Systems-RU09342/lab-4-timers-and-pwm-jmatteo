# Software Debouncing
When the activation button on each board is pressed, it toggles LED P1.0. Using heardware timers and interrupts, the boards can tell whether or not the button has bounced. It does this by toggling a timer when the button is pressed. If the timer is 0, it toggles the LED. If not, it ignores the press. The timer is set to count down for 2ms.

## BOARDS USED
MSP430G2553
MSP430FR6989

On the G2 board, the botton which needs to be pressed is button P1.3 while on th 6989 it is button P1.1.
