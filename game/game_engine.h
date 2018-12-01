#define GREEN_LED BIT6
#define SIZE 8

extern char game_running, game_end; 

void game_init(); 
short game_run(short counter); 
void game_end(); 