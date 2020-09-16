#include "BTInterface.h"
#include <SoftwareSerial.h>

SoftwareSerial blue_serial(8, 9);
bt_interface bt_i = {4, "INIT", &blue_serial};
char in = ' ';

void setup() {
  Serial.begin(9600);
  initialise_interface(&bt_i);
}

void loop() {
  comm_status rx_status = NOP;
  
  if (Serial.available() > 0) {
    in = Serial.read();
    rx_status = build_frame(&bt_i, in);
    Serial.write(in);
  }

  if (rx_status == TRANSMIT) transmit_frame(&bt_i);

  if (blue_serial.available() > 0)
    Serial.write(blue_serial.read());
}
