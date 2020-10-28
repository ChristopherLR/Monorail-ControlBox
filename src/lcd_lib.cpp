#include "lcd_lib.h"

// Function to replace characters in a given LCD line
void insert_chars(char *insert, char *line, char length, char idx){
  for (char i = 0; i < length; i++) {
    line[i+idx] = insert[i];
  }
}

void display_open_close(open_close_state s, lcd_state_machine *sm) {
  char op[3] = "OP";
  char cl[3] = "CL";
  switch (s) {
    case DOOR_OPEN:
      // line2, index 2: OP
      insert_chars(op, sm->line2, 2, 14);
      insert_chars(op, sm->line1, 2, 14);
      break;
    case DOOR_CLOSE:
      // line2, index 2: CL
      insert_chars(cl, sm->line2, 2, 14);
      insert_chars(cl, sm->line1, 2, 14);
      break;
    default:
      break;
  }
}

void display_start_stop(start_stop_state s, lcd_state_machine *sm) {
    char st [] = "ST";
    char sp [] = "SP";
    char start [] = "START";
    char stop [] = "STOP";
  switch (s) {
    case B_START:
      // line2, index 7: ST
      insert_chars(start, sm->line1, 2, 5);
      insert_chars(st, sm->line1, 2, 14);
      break;
    case B_STOP:
      // line2, index 7: SP
      insert_chars(stop, sm->line1, 2, 5);
      insert_chars(sp, sm->line1, 2, 14);
      break;
    default:
      break;
  }
}

void display_emergency(lcd_state_machine *sm) {
    char ln1 [] = " EMERGENCY STOP ";
    char ln2 [] = "  PLEASE RESET  ";
    insert_chars(ln1, sm->line1, 16, 0);
    insert_chars(ln2, sm->line2, 16, 0);
}

void display_curr(east_west_state s, lcd_state_machine *sm) {
    char east[] = "EAST";
    char west[] = "WEST";
    switch (s) {
    case B_EAST:
      // line1, index 2: EAST
      insert_chars(east, sm->line2, 4, 2);
      break;
    case B_WEST:
      // line1, index 2: WEST
      insert_chars(west, sm->line2, 4, 2);
      break;
    default:
      break;
  }
}

void display_next(east_west_state s, lcd_state_machine *sm) {
//     char east[] = "EAST";
//     char west[] = "WEST";
//     switch (s) {
//     case B_EAST:
//       // line1, index 7: EAST
//       insert_chars(east, sm->line1, 4, 7);
//       break;
//     case B_WEST:
//       // line1, index 7: WEST
//       insert_chars(west, sm->line1, 4, 7);
//       break;
//     default:
//       break;
//   }
}

void display_debug(char debugInfo[], lcd_state_machine *sm) {
  // insert_chars(debugInfo, sm->line2, 4, 12);
}
