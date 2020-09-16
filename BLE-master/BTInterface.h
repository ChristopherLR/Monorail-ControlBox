#pragma once
#include <SoftwareSerial.h>
#define BUFFER_SIZE 20
#define BAUD_RATE 9600

typedef struct {
  char length;
  char msg[BUFFER_SIZE];
  SoftwareSerial *ss;
} bt_interface;

typedef enum { SUCCESS, FAILURE, OVERFLOW, TRANSMIT, NOP } comm_status;
comm_status initialise_interface(bt_interface *);
comm_status transmit_frame(bt_interface *);
comm_status build_frame(bt_interface *);

comm_status initialise_interface(bt_interface *frame) {
  frame->ss->begin(BAUD_RATE);
  comm_status status = transmit_frame(frame);
};

comm_status transmit_frame(bt_interface *frame) {
  if (frame->length == 0)
    return NOP;
  for (int i = 0; i < frame->length && i < BUFFER_SIZE; i++) {
    frame->ss->write(frame->msg[i]);
  }
  frame->ss->write("\r\n");
  frame->length = 0;
  return SUCCESS;
}

comm_status build_frame(bt_interface *frame, char c) {
  if (frame->length >= BUFFER_SIZE)
    return OVERFLOW;
  if (c == '\r')
    return TRANSMIT;
  if (c == '\n')
    return NOP;
  frame->msg[frame->length] = c;
  frame->length++;
  return SUCCESS;
}
