#ifndef buzzer_included
#define buzzer_included
// void delay_ms(unsigned int ms); 
// void delay_us(unsigned int us);
// void beep(unsigned short note);  

void buzzer_init();
void buzzer_advance_frequency();
void buzzer_set_period(short cycles);

#endif // included
