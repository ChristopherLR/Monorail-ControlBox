#include <SoftwareSerial.h>

#pragma once
#define BUFFER_SIZE 20
#define BAUD_RATE 38400

typedef struct {
  char length;
  char msg[BUFFER_SIZE];
  SoftwareSerial *ss;
} bt_interface;

typedef enum { SUCCESS, FAILURE, OVERFLOW, TRANSMIT, NOP } comm_status;
comm_status initialise_interface(bt_interface *);
comm_status transmit_frame(bt_interface *);
comm_status build_frame(bt_interface *, char c);
comm_status quick_transmit(bt_interface *, char c);
void reset_frame(bt_interface);
