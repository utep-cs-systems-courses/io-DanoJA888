//Alternate LEDs from Off, Green, and Red
#include <msp430.h>
#include "libTimer.h"
#include "led.h"

int main(void) {
  P1DIR |= LEDS;
  P1OUT &= ~LED_GREEN;
  P1OUT |= LED_RED;

  configureClocks();		/* setup master oscillator, CPU & peripheral clocks */
  enableWDTInterrupts();	/* enable periodic interrupt */
  
  or_sr(0x18);			/* CPU off, GIE on */
}

// global state vars that control blinking
int blinkLimitR = 8;  // duty cycle = 1/blinkLimit
int blinkCountR = 0;  // cycles 0...blinkLimit-1
int secondCount = 0; // state var representing repeating time 0…1s

int blinkLimitG = 8;
int blinkCountG = 0;

void
__interrupt_vec(WDT_VECTOR) WDT()	/* 250 interrupts/sec */
{
  // handle blinking 
  blinkCountR ++;
  if (blinkCountR >= blinkLimitR) { // on for 1 interrupt period
    blinkCountR = 0;
    P1OUT &= ~LED_GREEN;
  } else		          // off for blinkLimit - 1 interrupt periods
    P1OUT |= LED_GREEN;

  blinkCountG ++;
  if(blinkCountG >= blinkLimitG){
    blinkCountG = 0;
    P1OUT &= ~LED_RED;
  }
  else
    P1OUT |= LED_RED;
  

  // measure a second
  secondCount ++;
  if (secondCount >= 250) {  // once each second
    secondCount = 0;
    blinkLimitR ++;
    blinkLimitG ++;           // reduce duty cycle
    if (blinkLimitR >= 8)     // but don't let duty cycle go below 1/7.
      blinkLimitR = 0;
    if (blinkLimitG >= 8)
      blinkLimitG = 0;
  }
} 

