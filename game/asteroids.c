#include <stdlib.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include <shape.h>
#include <abCircle.h>

Layer new_layer = {
    (AbShape *)&circle6,
    {((rand()% (screenWidth-8+1))+1)*-1,(rand()% (screenWidth-8+1))+1},
    {0,0},{0,0},
    COLOR_CYAN,
    new_layer2,
};

Layer new_layer2 = {
    (AbShape *)&circle8,
    {((rand()% (screenWidth-8+1))+1)*-1,(rand()% (screenWidth-8+1))+1},
    {0,0},{0,0},
    COLOR_CYAN,
    0
};

void init_asteroids(MovLayer *result)
{
  //Layer as; 
    //as.abShape = ((rand() % 2)==0) ? (AbShape *)&circle8 : (AbShape *)&circle6; 
    //as.pos.axes[0] = (rand()% (screenWidth-0+1))+1; 
    //as.pos.axes[0] = (rand()% (screenWidth-0+1))+1; 
    //as.color = COLOR_CYAN; 
    result->abShape = &new_layer; 
    result->velocity.axes[0] = 0; 
    result->velocity.axes[1] = -2;
    result->next = 0; 
}

void addLayer(MovLayer *ml)
{
    Layer new_layer = {
        ((rand()%2)==0) ? (AbShape *)&circle8 : (AbShape *)&circle6,
        {((rand()% (screenWidth-8+1))+1)*-1,(rand()% (screenWidth-8+1))+1},
        {0,0},{0,0},
        COLOR_CYAN,
        0
    }; 

    if (ml->layer == 0){
        ml->layer = &new_layer; 
        return; 
    } 

    Layer* temp = ml->layer; 
    while (temp) temp = temp->next;
    *temp = new_layer;
    return;
}
