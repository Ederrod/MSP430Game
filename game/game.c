#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include <shape.h>

AbRect rect5 = {abRectGetBounds, abRectCheck, 8, 8}; 


Layer layer0 = {
    (AbShape*)&rect5,
    {screenWidth/2,screenHeight-8},
    {0,0},{0,0},
    COLOR_RED,
    0,
}; 

u_int bgColor = COLOR_BLUE;
int redraw_screen = 1; 

int 
main()
{
    configureClocks();
    lcd_init();
    shapeInit();
    p2sw_init(1); // Initialize the switches to be used 

    shapeInit(); 
    clearScreen(COLOR_BLUE);

    layerDraw(&layer0); 

    enableWDTInterrupts(); /** Enable periodic interrupts */ 
    or_sr(0x8);            /** ebable interrupts */

    for(;;){
        while (!redraw_screen){
            
        }
        // Move our enemy ships
    }
}

/**
 * Create state machine for button press for our player ship
 * BTN0 move right 
 * BTN4 move left
 */
int switch_state = 0; 

void 
wdt_c_handler()
{
    u_int switches = p2sw_read(); 
    if (switches & (1<<0)){
        // Move left
    }

    if (switches & (1<<3)){
        // Move right
    }
}