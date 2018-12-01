#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include <shape.h>
#include <abCircle.h>

#include "mov_layer.h"
 

#define SIZE 8
#define GREEN_LED BIT6

char game_running; 

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
};

// Asteroids
Layer asteroid8 = {
  (AbShape *) &circle6,
  {SIZE, SIZE},/**< center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_BLACK,
  0
};

Layer asteroid7 = {
  (AbShape *) &circle6,
  {SIZE+SIZE+1, SIZE},/**< center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_BLACK,
  0,
};

Layer asteroid6 = {
  (AbShape *) &circle6,
  {SIZE+SIZE+SIZE+1, SIZE},/**< center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_BLACK,
  0,
};

Layer asteroid5 = {
  (AbShape *) &circle6,
  {SIZE+SIZE+SIZE+SIZE+1, SIZE},/**< center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_BLACK,
  0,
};

Layer asteroid4 = {
  (AbShape *) &circle6,
  {SIZE+SIZE+SIZE+SIZE+SIZE+1, SIZE},/**< center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_BLACK,
  0,
};

Layer asteroid3 = {
  (AbShape *) &circle6,
  {SIZE+SIZE+SIZE+SIZE+SIZE+SIZE+1, SIZE},/**< center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_BLACK,
  0,
};

Layer asteroid2 = {
  (AbShape *) &circle6,
  {SIZE+SIZE+SIZE+SIZE+SIZE+SIZE+SIZE+1, SIZE},/**< center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_BLACK,
  0,
};

/* initial value of {0,0} will be overwritten */
MovLayer ml0 = { &layer0, {1,0}, 0 };  // Player

MovLayer as = {&asteroid2, {0,-2}, 0}; // Asteroid
MovLayer as1 = {&asteroid3, {0,-2}, 0};
MovLayer as2 = {&asteroid4, {0,-2}, 0};
MovLayer as3 = {&asteroid5, {0,-2}, 0};
MovLayer as4 = {&asteroid6, {0,-2}, 0};
MovLayer as5 = {&asteroid7, {0,-2}, 0};
MovLayer as6 = {&asteroid8, {0,-2}, 0};

Region fieldFence;

u_int bgColor = COLOR_BLUE;    
int redrawScreen = 1;
void main()
{
  P1DIR |= GREEN_LED;		/**< Green led on when CPU on */		
  P1OUT |= GREEN_LED;

  game_running = 1; 

  configureClocks();
  lcd_init();
  shapeInit();
  p2sw_init(15);

  shapeInit();

  layerInit(&layer0);
  layerInit(&asteroid2);
  layerInit(&asteroid3);
  layerInit(&asteroid4);
  layerInit(&asteroid5);
  layerInit(&asteroid6);
  layerInit(&asteroid7);
  layerInit(&asteroid8);

  layerDraw(&layer0);
  layerDraw(&asteroid2);
  layerDraw(&asteroid3);
  layerDraw(&asteroid4);
  layerDraw(&asteroid5);
  layerDraw(&asteroid6);
  layerDraw(&asteroid7);
  layerDraw(&asteroid8);
  
  //layerInit((as.layer));
  // layerDraw((as.layer)); 
  // layerInit(&layer1);
  // layerDraw(&layer1);

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
      movLayerDraw(&as, &asteroid2); 
      movLayerDraw(&as1, &asteroid3);
      movLayerDraw(&as2, &asteroid4);
      movLayerDraw(&as3, &asteroid5);
      movLayerDraw(&as4, &asteroid6);
      movLayerDraw(&as5, &asteroid7);
      movLayerDraw(&as6, &asteroid8); 
  }
}

/** Watchdog timer interrupt handler. 15 interrupts/sec */
void wdt_c_handler()
{
  static short count = 0;
  P1OUT |= GREEN_LED;		      /**< Green LED on when cpu on */
  count ++;
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
      mlAsteroidAdvance(&as, &fieldFence);
      mlAsteroidAdvance(&as1, &fieldFence);
      mlAsteroidAdvance(&as2, &fieldFence);
      mlAsteroidAdvance(&as3, &fieldFence);
      mlAsteroidAdvance(&as4, &fieldFence);
      mlAsteroidAdvance(&as5, &fieldFence);
      mlAsteroidAdvance(&as6, &fieldFence);

      char end = collisionDetection(&as, &ml0) && 
      collisionDetection(&as1, &ml0) && 
      collisionDetection(&as2, &ml0) &&
      collisionDetection(&as3, &ml0) &&
      collisionDetection(&as4, &ml0) &&
      collisionDetection(&as5, &ml0) &&
      collisionDetection(&as6, &ml0);
      if (end)
        drawString5x7(20,20, "You Lost", COLOR_GREEN, COLOR_BLUE);
      if (p2sw_read())
        redrawScreen = 1;
      count = 0;
  }
  P1OUT &= ~GREEN_LED;		    /**< Green LED off when cpu off */
}
