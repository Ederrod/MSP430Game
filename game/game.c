#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "shape.h"

AbRect rect5 = {abRectGetBounds, abRectCheck, {5,5}}; 


Layer layer0 = {
    (AbShape*)&rect5,
    {screenWidth/2,screenHeight/2},
    {0,0},
    {0,0},
    COLOR_GOLD,
    0
}; 


int 
main()
{
    configureClocks();
    lcd_init();
    shapeInit();

    clearScreen(COLOR_BLUE);

    layerDraw(&layer0); 
}