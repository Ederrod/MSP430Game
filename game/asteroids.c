#include <stdlib.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include <shape.h>
#include <abCircle.h>



MovLayer init_asteroids()
{
  //Layer as; 
    //as.abShape = ((rand() % 2)==0) ? (AbShape *)&circle8 : (AbShape *)&circle6; 
    //as.pos.axes[0] = (rand()% (screenWidth-0+1))+1; 
    //as.pos.axes[0] = (rand()% (screenWidth-0+1))+1; 
    //as.color = COLOR_CYAN; 

    MovLayer mAs; 
    //mAs.layer = &as; 
    mAs.velocity.axes[0]=0; 
    mAs.velocity.axes[0]=-2; 

    return mAs; 
}

