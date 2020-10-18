#include <stdint.h>

typedef enum { DOOR_OPEN, DOOR_CLOSE, OC_NONE } open_close_state;
typedef enum { B_EAST, B_WEST, EW_NONE } east_west_state;
typedef enum { B_START, B_STOP, SS_NONE } start_stop_state;
typedef enum { B_EMERGENCY, B_NOT_EMERGENCY } emergency_state;

typedef struct {
  open_close_state cur_state;
  open_close_state nx_state;
  bool to_tx;
  bool is_pressed;
} open_close_button;

typedef struct {
  east_west_state cur_state;
  east_west_state nx_state;
  bool to_tx;
  bool is_pressed;
} east_west_button;

typedef struct {
  start_stop_state cur_state;
  start_stop_state nx_state;
  bool to_tx;
  bool is_pressed;
} start_stop_button;

typedef struct {
  emergency_state cur_state;
  bool to_tx;
  bool is_pressed;
} emergency;

typedef struct {
  unsigned int btn_delay;
  unsigned int last_pressed;
  open_close_button open_close_btn;
  east_west_button east_west_btn;
  start_stop_button start_stop_btn;
  emergency emergency_btn;
} btns_state_machine;

typedef struct {
  char line1[16];
  char line2[16];
  bool to_tx;
} lcd_state_machine;

