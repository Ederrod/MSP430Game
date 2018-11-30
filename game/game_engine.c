#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include <shape.h>
#include <abCircle.h>

#include "mov_layer.h"


static enum {Running = 1, Stop = 0}game_state; 

AbRect rect10 = {abRectGetBounds, abRectCheck, {SIZE, SIZE/2}};

AbRectOutline fieldOutline = {	/* playing field */
  abRectOutlineGetBounds, abRectOutlineCheck,   
  {screenWidth/2, screenHeight/2}
};

Layer fieldLayer = {		/* playing field as a layer */
  (AbShape *) &fieldOutline,
  {screenWidth/2, screenHeight/2},/**< center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_BLACK,
  0
};

Layer layer0 = {		/**< Layer with a red square */
  (AbShape *)&rect10,
  {screenWidth/2, screenHeight-SIZE/2}, /**< center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_RED,
  &fieldLayer,
}

Layer layer1 = {
  (AbShape *)&circle6,
  {SIZE, SIZE}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_ORANGE,
  0
};

Layer layer2 = {
  (AbShape *)&circle4,
  {SIZE+SIZE, SIZE}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_ORANGE,
  0
};
/* initial value of {0,0} will be overwritten */
MovLayer ml0 = { &layer0, {1,0}, 0 };  // Player


MovLayer ml1 = { &layer1, {0,-2}, 0 }; // asteroid


Region fieldFence;

u_int bgColor = COLOR_BLUE;    
int redrawScreen = 1;           

void game_init()
{
    P1DIR |= GREEN_LED;		/**< Green led on when CPU on */		
    P1OUT |= GREEN_LED;

    game_state = 1; 

    configureClocks();
    lcd_init();
    shapeInit();
    p2sw_init(15);

    shapeInit();

    layerInit(&layer0);
    layerDraw(&layer0);

    layerInit(&layer1);
    layerDraw(&layer1);

    layerInit(&layer2);
    layerDraw(&layer2);

    layerGetBounds(&fieldLayer, &fieldFence);

    enableWDTInterrupts();      /**< enable periodic interrupt */
    or_sr(0x8);	              /**< GIE (enable interrupts) */

    for(;;) { 
        while (!redrawScreen) { /**< Pause CPU if screen doesn't need updating */
        P1OUT &= ~GREEN_LED;    /**< Green led off witHo CPU */
        or_sr(0x10);	      /**< CPU OFF */
        }
        P1OUT |= GREEN_LED;       /**< Green led on when CPU on */
        redrawScreen = 0;
        movLayerDraw(&ml0, &layer0);
        movLayerDraw(&ml1, &layer1); 
    }
}

short game_run(short count)
{
    u_int switches = p2sw_read();

    if((switches & (1<<0)) == 0){
        mlPlayerAdvanceLeft(&ml0, &fieldFence);
        redrawScreen = 1;  
    }

    if((switches & (1<<3)) == 0){
        mlPlayerAdvanceRight(&ml0, &fieldFence);
        redrawScreen = 1;  
    }

    if (count == 15) {
        if (p2sw_read())
        redrawScreen = 1;
    } 

    if (count == 30){
        mlAsteroidAdvance(&ml1, &fieldFence); 
        char end = collisionDetection(&ml1, &ml0);
        if (end)
            game_state = 0; 
        if (p2sw_read())
            redrawScreen = 1;
        count = 0;
    }
}

void game_end()
{

}