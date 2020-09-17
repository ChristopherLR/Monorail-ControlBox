#include <SoftwareSerial.h>

#pragma once
#define BUFFER_SIZE 20
#define BAUD_RATE 38400

typedef enum { SUCCESS, FAILURE, OVERFLOW, TRANSMIT, NOP } comm_status;
typedef enum { EASTRX, WESTRX, STARTRX, STOPRX, OPENRX, CLOSERX, EMERGENCYRX, NONERX } msg_rx;
typedef enum { EASTTX, WESTTX, STARTTX, STOPTX, OPENTX, CLOSETX, EMERGENCYTX, NONETX } msg_tx;

typedef struct {
  char update;
  char length;
  msg_rx rx;
  msg_tx tx;
  char msg[BUFFER_SIZE];
  SoftwareSerial *ss;
} bt_interface;

comm_status initialise_interface(bt_interface *);
comm_status transmit_frame(bt_interface *);
comm_status build_frame(bt_interface *, char c);
comm_status quick_transmit(bt_interface *, char c);
void reset_frame(bt_interface);
