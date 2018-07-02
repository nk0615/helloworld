/*      Project: Traffic Light Simulation
 *      Tool: TI Launchpad MSP430F5529
 *      language used: C
 *      Date: Nov 2017
 *      Owner: Ng, Ka In
 */

#include <msp430.h> 

int extend = 0; // extend green duration
int timer = 0; // current timer (in 0.5 secs)
int green = 0; // current color of light. when red is on green = 0.


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    // init red led
    P1DIR |= BIT0;
    P1OUT |= BIT0;  // initially in red state

    // init green led
    P4DIR |= BIT7;
    P4OUT &= ~BIT7; // green is off

    // init button
    P1DIR &= ~BIT1; // input mode
    P1REN |= BIT1;
    P1OUT |= BIT1; // pull-up resistor mode
    P1IES |= BIT1; // falling-edge select
    P1IFG &= ~BIT1; // reset flag
    P1IE |= BIT1; // interrupt enable

    // init timer
    TA0CCTL0 = CCIE;
    TA0CCR0 = 16000; // 0.5 sec timer
    TA0CTL = TASSEL_1 + ID_0 + MC_1;

    // enable interrupt
    _BIS_SR(GIE);

    return 0;
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void) {
    timer++;

    if (green) { // when the
        if (timer >= 20 || (extend == 0 && timer >= 10)) {
            timer = 0;
            extend = 0; //resets timer + extend + green LED.
            green = 0;

            P1OUT |= BIT0; // red LED back on
            P4OUT &= ~BIT7; // green LED off
        }
        else {
            P4OUT ^= BIT7; // blink(toggle) green light
        }
    }
    else { //when the red is on / green is not on, do the following
        if (timer >= 10) { //end the red light after 5 sec
            timer = 0; //resets the timer to 0
            green = 1; //green LED should be on

            P1OUT &= ~BIT0; // turn off red
            P4OUT |= BIT7; // turn on green
        }
    }
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {
    if (P1IFG & BIT1)
        extend = 1; // set extend flag to 1
    P1IFG &= ~BIT1; // reset flag
}

