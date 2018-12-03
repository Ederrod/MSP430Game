#include "game_sm.h"

char game_running, game_end; 

void game_init()
{
    game_running = 1; 
    game_end = 0; 
}

void game_state_update(char update)
{
    if (update){
        if (game_running){
            game_end = 1; 
            game_running = 0; 
        }
        else{
            game_running = 1; 
            game_end = 0; 
        }
    }
}
