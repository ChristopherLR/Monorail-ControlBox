#pragma once

//Reads the msg from rx
char read_msg(){
  int in = NONERX;
  if (blue_serial.available() > 0) in = blue_serial.read();
  return in;
}

//Sends the value of tx to the other arduino
comm_status send_msg() { 
  switch (bt_i.tx) {
    case EASTTX:
      quick_transmit(&bt_i, EASTTX);
      break;
    case WESTTX:
      quick_transmit(&bt_i, WESTTX);
      break;
    case STARTTX:
      quick_transmit(&bt_i, STARTTX);
      break;
    case STOPTX:
      quick_transmit(&bt_i, STOPTX);
      break;
    case OPENTX:
      quick_transmit(&bt_i, OPENTX);
      break;
    case CLOSETX:
      quick_transmit(&bt_i, CLOSETX);
      break;
    case EMERGENCYTX:
      quick_transmit(&bt_i, EMERGENCYTX);
      break;
    default:
      break;  
  };
  return SUCCESS;
}

//Update message with what needs to be sent each cycle
void send_east() { bt_i.tx = EASTTX; }
void send_west() { bt_i.tx = WESTTX;  }
void send_start() { bt_i.tx = STARTTX; }
void send_stop() { bt_i.tx = STOPTX; }
void send_doors_open() {  bt_i.tx = OPENTX; }
void send_doors_closed() { bt_i.tx = CLOSETX; }
void send_emergency() { bt_i.tx = EMERGENCYTX; }
void send_not_emergency() { bt_i.tx = NONETX; }
