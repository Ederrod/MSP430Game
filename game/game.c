#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include <shape.h>
#include <abCircle.h>

#include "game_sm.h"
#include "mov_layer.h"
#include "buzzer.h"
 

#define SIZE 8
#define GREEN_LED BIT6 

AbRect rect10 = {abRectGetBounds, abRectCheck, {SIZE, SIZE/2}};

AbRectOutline fieldOutline = {	/* playing field */
  abRectOutlineGetBounds, abRectOutlineCheck,   
  {screenWidth/2-1, screenHeight/2-1}
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
  &fieldLayer
};

// Asteroids

Layer asteroid6 = {
  (AbShape *) &circle6,
  {SIZE, SIZE},/**< center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_BLACK,
  &layer0
};

Layer asteroid5 = {
  (AbShape *) &circle6,
  {SIZE*4, SIZE},/**< center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_BLACK,
  &asteroid6
};

Layer asteroid4 = {
  (AbShape *) &circle6,
  {screenWidth-SIZE, SIZE},/**< center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_BLACK,
  &asteroid5
};

Layer asteroid3 = {
  (AbShape *) &circle6,
  {screenWidth/2, SIZE},/**< center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_BLACK,
  &asteroid4
};

Layer asteroid2 = {
  (AbShape *) &circle6,
  {screenWidth-(SIZE*4), SIZE},/**< center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_BLACK,
  &asteroid3
};


void respawn(MovLayer *as)
{ 
  Region asRe;
  for (; as; as = as->next){
    abShapeGetBounds(as->layer->abShape,&as->layer->pos,&asRe);
    if (asRe.topLeft.axes[1]>=screenHeight){
      as->layer->posNext.axes[0] = (as->layer->pos.axes[0]+10 >= screenWidth)? 0 : as->layer->pos.axes[0]+10;
      as->layer->posNext.axes[1] = 0;
    } 
  }
}
/* initial value of {0,0} will be overwritten */
MovLayer ml0 = { &layer0, {1,0}, 0 };  // Player

MovLayer as = {&asteroid2, {0,1}, 0}; // Asteroid
MovLayer as1 = {&asteroid3, {0,1}, 0};
MovLayer as2 = {&asteroid4, {0,1}, 0};
MovLayer as3 = {&asteroid5, {0,1}, 0};
MovLayer as4 = {&asteroid6, {0,1}, 0};

Region fieldFence;

u_int bgColor = COLOR_BLUE;    
int redrawScreen = 1;
void main()
{
  P1DIR |= GREEN_LED;		/**< Green led on when CPU on */		
  P1OUT |= GREEN_LED;

  configureClocks();
  lcd_init();
  //buzzer_init(); 
  game_init(); 
  shapeInit();
  p2sw_init(15);

  shapeInit();
  clearScreen(COLOR_BLUE);  
  layerInit(&layer0); 
  layerInit(&asteroid2);
  layerInit(&asteroid3);
  layerInit(&asteroid4);
  layerInit(&asteroid5);
  layerInit(&asteroid6);
 
  layerDraw(&asteroid2);
 
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
      respawn(&as); 
      respawn(&as1);
      respawn(&as2);
      respawn(&as3);
      respawn(&as4);
  }
}

/** Watchdog timer interrupt handler. 15 interrupts/sec */
void wdt_c_handler()
{
  if (game_running){
    static short count = 0;
    P1OUT |= GREEN_LED;		      /**< Green LED on when cpu on */
    u_int switches = p2sw_read();

    if((switches & (1<<0)) == 0){
      mlPlayerAdvanceLeft(&ml0, &fieldFence);
      // buzzer_advance_frequency(); 
      redrawScreen = 1;  
    }

    if((switches & (1<<3)) == 0){
      mlPlayerAdvanceRight(&ml0, &fieldFence);
      //buzzer_advance_frequency();
      redrawScreen = 1;  
    } 

    if (count == 15){
      mlAsteroidAdvance(&as);
      mlAsteroidAdvance(&as1);
      mlAsteroidAdvance(&as2);
      mlAsteroidAdvance(&as3);
      mlAsteroidAdvance(&as4);
      
      char end = collisionDetection(&as, &ml0) ||
      collisionDetection(&as1, &ml0) || 
      collisionDetection(&as2, &ml0) ||
      collisionDetection(&as3, &ml0) ||
      collisionDetection(&as4, &ml0);
      game_state_update(end); 
      if(p2sw_read()){
        redrawScreen = 1; 
      }
      count = 0; // reset counter 
    }
    count++;
    P1OUT &= ~GREEN_LED;
  }		 
  if (game_end) {
    drawString5x7(20,20,"YOU LOST",COLOR_GREEN,bgColor);
    drawString5x7(20,40,"Press button 1 to play again",COLOR_GREEN,bgColor);
    u_int switches = p2sw_read();
    if((switches & (1<<0)) == 0){
      game_state_update(1); 
      redrawScreen = 1;  
    }
  }
}
