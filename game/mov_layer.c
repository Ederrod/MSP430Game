#include <libTimer.h>

#include "mov_layer.h"

void movLayerDraw(MovLayer *movLayers, Layer *layers)
{
  int row, col;
  MovLayer *movLayer;

  and_sr(~8);			/**< disable interrupts (GIE off) */
  for (movLayer = movLayers; movLayer; movLayer = movLayer->next) { /* for each moving layer */
    Layer *l = movLayer->layer;
    l->posLast = l->pos;
    l->pos = l->posNext;
  }
  or_sr(8);			/**< disable interrupts (GIE on) */


  for (movLayer = movLayers; movLayer; movLayer = movLayer->next) { /* for each moving layer */
    Region bounds;
    layerGetBounds(movLayer->layer, &bounds);
    lcd_setArea(bounds.topLeft.axes[0], bounds.topLeft.axes[1], 
		bounds.botRight.axes[0], bounds.botRight.axes[1]);
    for (row = bounds.topLeft.axes[1]; row <= bounds.botRight.axes[1]; row++) {
      for (col = bounds.topLeft.axes[0]; col <= bounds.botRight.axes[0]; col++) {
        Vec2 pixelPos = {col, row};
        u_int color = bgColor;
        Layer *probeLayer;
        for (probeLayer = layers; probeLayer; 
            probeLayer = probeLayer->next) { /* probe all layers, in order */
          if (abShapeCheck(probeLayer->abShape, &probeLayer->pos, &pixelPos)) {
            color = probeLayer->color;
            break; 
          } /* if probe check */
        } // for checking all layers at col, row
        lcd_writeColor(color); 
      } // for col
    } // for row
  } // for moving layer being updated
}	

void mlPlayerAdvanceLeft(MovLayer *ml, Region *fence)
{
  Vec2 newPos;
  u_char axis;
  Region shapeBoundary;
  for (; ml; ml = ml->next) {
    vec2Sub(&newPos, &ml->layer->posLast, &ml->velocity);
    abShapeGetBounds(ml->layer->abShape, &newPos, &shapeBoundary);
    //Move left
    if ((shapeBoundary.topLeft.axes[0] < fence->topLeft.axes[0]) ||
      (shapeBoundary.botRight.axes[0] > fence->botRight.axes[0]) ) {
      int velocity = -ml->velocity.axes[0];
      newPos.axes[0] -= (2*velocity);
    } 
    ml->layer->posNext = newPos;
  } 
}

void mlPlayerAdvanceRight(MovLayer *ml, Region *fence)
{
  Vec2 newPos;
  u_char axis;
  Region shapeBoundary;
  for (; ml; ml = ml->next) {
    vec2Add(&newPos, &ml->layer->posNext, &ml->velocity); 
    abShapeGetBounds(ml->layer->abShape, &newPos, &shapeBoundary);
    if ((shapeBoundary.topLeft.axes[0] < fence->topLeft.axes[0]) ||
    (shapeBoundary.botRight.axes[0] > fence->botRight.axes[0])){
      int velocity = ml->velocity.axes[0] = 0; 
      newPos.axes[0] += (2*velocity); 
    }
    ml->layer->posNext = newPos;
  } 
}

void mlAsteroidAdvance(MovLayer *ml, Region *fence)
{
  Vec2 newPos;
  u_char axis;
  Region shapeBoundary;
  for (; ml; ml = ml->next){
    vec2Sub(&newPos, &ml->layer->posNext, &ml->velocity);
    abShapeGetBounds(ml->layer->abShape, &newPos, &shapeBoundary);
    for (axis = 0; axis < 2; axis ++){
      if ((shapeBoundary.topLeft.axes[axis] < fence->topLeft.axes[axis]) || 
          (shapeBoundary.botRight.axes[axis] > fence->botRight.axes[axis])) { 
	        int velocity = -ml->velocity.axes[axis]; // = 0;
      }	
        ml->layer->posNext = newPos;
    }       
  } 
}

char collisionDetection(MovLayer *asteroids, MovLayer *player)
{
  char collision = 0; 
  u_char axis;
  Region asteroidBound;  
  Region playerBound;  
  for (; asteroids; asteroids = asteroids->next){
    abShapeGetBounds(asteroids->layer->abShape, &asteroids->layer->pos, &asteroidBound); 
    abShapeGetBounds(player->layer->abShape, &player->layer->pos, &playerBound); 
    for (axis = 0; axis < 2; axis++){
      if ((asteroidBound.botRight.axes[axis] >= playerBound.topLeft.axes[axis]) &&
          (asteroidBound.botRight.axes[axis] <= playerBound.botRight.axes[axis])){
        collision = 1; 
      }
    }
  }
  return collision; 
}