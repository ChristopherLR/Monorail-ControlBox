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

void configure_clock2(){
	TIMSK2 = 0b00000100; //Setting flag on compare b
	/* TCCR2B Timer/Counter Control Reg B
	 * [FOC2A][FOC2B][-][-][WGM22][CS22][CS21][CS20]
	 */
	TCCR2B = 0; // turning clock off

	/* TCCR2A Timer/Counter Control Reg A
	 * [COM2A1][COM2A0][COM2B1][COM2B0][-][-][WGM21][WGM20]
	 */
	TCCR2A = 0; // Setting waveform to normal op

	OCR2B = 255; // Setting max compare

	TIFR2 = 0b00000100; // ISR on compare match b

	/*
	 * (16,000,000/1024)/255 = 0.01632
	 * 1/0.01632 = 61 -> need 61 for 1 second
	 */
	TCCR2B = 0b00000111; // Setting prescaler to 1024

	TCNT2 = 0; // Setting clock to 0
}