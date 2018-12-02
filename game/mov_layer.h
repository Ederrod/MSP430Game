#include <shape.h>

/** Moving Layer
 *  Linked list of layer references
 *  Velocity represents one iteration of change (direction & magnitude)
 */
typedef struct MovLayer_s {
  Layer *layer;
  Vec2 velocity;
  struct MovLayer_s *next;
} MovLayer;

void movLayerDraw(MovLayer *movLayers, Layer *layers);
void mlPlayerAdvanceLeft(MovLayer *ml, Region *fence);
void mlPlayerAdvanceRight(MovLayer *ml, Region *fence);   
void mlAsteroidAdvance(MovLayer *ml); 
char collisionDetection(MovLayer *asteroids, MovLayer *player);
//void respawn(MovLayer *as);  
