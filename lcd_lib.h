#ifndef LCD_LIB
#define LCD_LIB

#include "buttons.h"

// Function to replace characters in a given LCD line
void insert_chars(char* insert, char *line, char length, char index);
void display_open_close(open_close_state s, lcd_state_machine *sm);
void display_start_stop(start_stop_state s, lcd_state_machine *sm);
void display_emergency(emergency_state s, lcd_state_machine *sm);
void display_curr(east_west_state s, lcd_state_machine *sm);
void display_next(east_west_state s, lcd_state_machine *sm);

#endif