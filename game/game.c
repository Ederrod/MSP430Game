#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "shape.h"

int 
main()
{
    configureClocks();
    lcd_init();
    shapeInit();

    clearScreen(COLOR_BLUE);

}