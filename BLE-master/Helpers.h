#pragma once
// FUNCTIONS TO SEND STATE DATA
void send_stop() { quick_transmit(&bt_i, STOP); }

void send_go() { quick_transmit(&bt_i, START); }

void send_east() { quick_transmit(&bt_i, EAST); }

void send_west() { quick_transmit(&bt_i, WEST); }

void send_emergency() { quick_transmit(&bt_i, EMERGENCY); }

void send_not_emergency() { quick_transmit(&bt_i, NONE); }

void send_doors_open() { quick_transmit(&bt_i, OPEN); }

void send_doors_closed() { quick_transmit(&bt_i, CLOSE); }
