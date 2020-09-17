#include "BTInterface.h"
#include <SoftwareSerial.h>

//These will change to what pins each button is actually connected to
#define GOBUTTON 2
#define DOORBUTTON 11
#define EMEGENCYBUTTON 12
#define DIRECTIONBUTTON 13


typedef enum {
  BUTTONTRANSMIT,
  WAIT,
  ETC  
} btn_state;

typedef struct {
  boolean update;
  btn_state current_state;
  btn_state next_state;  
} button_sm;

button_sm go_button {0, WAIT, WAIT};
button_sm ew_button {0, WAIT, WAIT};
button_sm dr_button {0, WAIT, WAIT};
button_sm em_button {0, WAIT, WAIT};

SoftwareSerial blue_serial(8, 9);
bt_interface bt_i = {0, 4, NONERX, NONETX, "INIT", &blue_serial};
#include "Helpers.h"
char in = ' ';

void setup() {
  Serial.begin(38400);
  initialise_interface(&bt_i);
  pinMode(2,INPUT);

  //These four intterrupts occur when their respective button in used
  attachInterrupt(digitalPinToInterrupt(GOBUTTON),toggle_go_update,FALLING);
  attachInterrupt(digitalPinToInterrupt(DOORBUTTON),toggle_dir_ipdate,FALLING);
  attachInterrupt(digitalPinToInterrupt(EMEGENCYBUTTON),toggle_door_update,FALLING);
  attachInterrupt(digitalPinToInterrupt(DIRECTIONBUTTON),toggle_emergency_update,FALLING);
}

/*
 * The following four functions get executed when their respective button has been hit
 * They will update the update field in the button_sm struct to show that there has been 
 * a change.
 */
void toggle_go_update(){
  go_button.update = 1;
}

void toggle_dir_ipdate(){
  go_button.update = 1;
}

void toggle_door_update(){
  go_button.update = 1;
}

void toggle_emergency_update(){
  go_button.update = 1;
}

void loop() {
  comm_status rx_status = NOP;
  int rx = read_msg(); //Read the message coming in from the Motor controller, in the form of msg_rx enum found in BTInterface.h

  if (Serial.available() > 0) {
    in = Serial.read();
    rx_status = build_frame(&bt_i, in);
    Serial.write(in);
  }

  if (rx_status == TRANSMIT) transmit_frame(&bt_i);

}
