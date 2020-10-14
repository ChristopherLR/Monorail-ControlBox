#include "BTInterface.h"
#include <SoftwareSerial.h>

SoftwareSerial blue_serial(8, 9);
bt_interface bt_i = {4, "INIT", &blue_serial};
char in = ' ';
comm_status comm_state = NOP;
#include "Helpers.h"

void setup() {
  Serial.begin(9600);
  initialise_interface(&bt_i);
}

void loop() {
  message blue_in = read_msg();
  print_message(blue_in);
}

void serialEvent(){
  comm_state = NOP;
  while (Serial.available()){
    char in = Serial.read();
    comm_state = build_frame(&bt_i, in);
    Serial.write(in);      
  }

  switch(comm_state) {
    case TRANSMIT:
      comm_state = transmit_frame(&bt_i);
      break;
    case OVERFLOW:
      reset_frame(&bt_i);
      break;
    case SUCCESS:
//       Serial.println(" sent");
      break;
    case FAILURE:
      // Serial.println(failed to send)
      break;
    case NOP:
      // Serial.println(doing nothing)
      break;
    default:
      // Serial.println(undefined behaviour)
      break;
  }
}

//function to read from bluetooth
message read_msg(){
  message blue_in = NONE;
  if (blue_serial.available() > 0) blue_in = blue_serial.read();
  return blue_in;
}

void print_message(message msg) {
  switch(msg) {
   case NONE:
//    Serial.println("NONE");
    break;
   case EAST:
    Serial.println("EAST");
    break;
   case WEST:
    Serial.println("WEST");
    break;
   case START:
    Serial.println("START");
    break;
   case STOP:
    Serial.println("STOP");
    break;
   case OPEN:
    Serial.println("OPEN");
    break;
   case CLOSE:
    Serial.println("CLOSE");
    break;
   case EMERGENCY:
    Serial.println("EMERGENCY");
    break;
   default:
    Serial.println("DEFAULT");
  }  
}
