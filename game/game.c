#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "shape.h"

AbRect rect5 = {abRectGetBounds, abRectCheck, 8, 8}; 


Layer layer0 = {
    (AbShape*)&rect5,
    {screenWidth/2,screenHeight-8},
    {0,0},{0,0},
    COLOR_RED,
    0,
}; 

u_int bgColor = COLOR_BLUE;

int 
main()
{
    configureClocks();
    lcd_init();
    shapeInit();

    clearScreen(COLOR_BLUE);

    layerDraw(&layer0); 
}
