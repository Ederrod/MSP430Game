 
#include <msp430.h>

#include "game_engine.h"

/** Initializes everything, enables interrupts and green LED, 
 *  and handles the rendering for the screen
 */
void main()
{
  game_init(); 
}

/** Watchdog timer interrupt handler. 15 interrupts/sec */
void wdt_c_handler()
{
  static short count = 0;
  P1OUT |= GREEN_LED;		      /**< Green LED on when cpu on */
  count ++;
  if (game_running){
    count = game_run(count); 
  }else {
    game_end(); 
  }
  P1OUT &= ~GREEN_LED;		    /**< Green LED off when cpu off */
}
