#include "mov_layer.h"
int switches_handler(MovLayer* ml0, Region *fieldFence, u_int state){
    int redrawScreen = 0; 
    if((state & (1<<0)) == 0){
      mlPlayerAdvanceLeft(ml0, fieldFence);
      redrawScreen = 1;  
    }

  if((state & (1<<3)) == 0){
      mlPlayerAdvanceRight(ml0, fieldFence);
      redrawScreen = 1;  
  }
  return redrawScreen; 
}