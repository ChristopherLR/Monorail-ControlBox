#include <Arduino.h>
#include "BTInterface.h"
#include <AltSoftSerial.h>
#include "buttons.h"

// Set DEBUG if you want serial printing for helpers
#define DEBUG // << Comment this out for performace testing

// INPUTS
#define SS_BUTTON 2
#define EW_BUTTON 4
#define OC_BUTTON 6
#define EMG_BUTTON 10

// OUTPUTS
#define SS_LED 3
#define EW_LED 5
#define OC_LED 7
#define EMG_LED 11

// STATE_MACHINE DECLARATION
open_close_button oc_button = { OC_NONE, OC_NONE, false, false };
east_west_button ew_button = { EW_NONE, EW_NONE, false, false };
emergency emg_button = { B_NOT_EMERGENCY, false, false };
start_stop_button ss_button = { SS_NONE, SS_NONE, false, false };

btns_state_machine buttons_sm = {
  1000, // Delay Between Presses (ms),
  0, // Last pressed init
  oc_button,
  ew_button,
  ss_button,
  emg_button
};

char next_state();
void check_state();
void panic();
char transmit_state();
message read_msg();
void print_msg(message);

// Counter / Timer configuration
#define TICKS_PER_SECOND 61
volatile unsigned char SYSTEM_COUNTER = 0;
volatile unsigned char SYSTEM_TICK = 0;
volatile unsigned char LED_COUNTER = 0;
volatile unsigned char BUTTON_PRESSED_TIM = 0;

// BLUETOOTH DECLARATION
AltSoftSerial blue_serial(8, 9);
bt_interface bt_i = {4, "INIT", &blue_serial};
char in = ' ';
comm_status comm_state = NOP;

// MISC
#define BAUD_RATE 9600
#include "Helpers.h"

void setup() {
  noInterrupts();
  Serial.begin(BAUD_RATE);
  initialise_interface(&bt_i);

  configure_clock2();

  // Turn on Interrupts for PORTD and PORTB 
  // Bit 0: Port B
  // Bit 2: Port D
  PCICR  = 0b00000101;

  // Enable Interrupts for PORTD:
  // Bit 2 - D2
  // Bit 4 - D4
  // Bit 6 - D6
  PCMSK2 = 0b01010100;

  // Enable Interrupts for PORTB
  // Bit 2 - D10
  PCMSK0 = 0b00000100;

  // PORTD CONFIG
  /*
  pinMode(SS_BUTTON, INPUT_PULLUP);
  pinMode(EW_BUTTON, INPUT_PULLUP);
  pinMode(OC_BUTTON, INPUT_PULLUP);
  pinMode(SS_LED, OUTPUT);
  pinMode(EW_LED, OUTPUT);
  pinMode(OC_LED, OUTPUT);
  ^^^^^^ EQUIVALENT BUT SMALLER
  */
  DDRD = 0b10101011;
  PORTD = 0b01010100;

  // PORTB CONFIG
  pinMode(EMG_BUTTON, INPUT_PULLUP);
  pinMode(EMG_LED, OUTPUT);
  interrupts();
}

void loop() {
  message blue_in = read_msg();

  print_msg(blue_in);

  check_state();
  char should_tx = next_state();

  if (should_tx) {
    transmit_state();
  }

}

// Interrupt Service Routine for PORT D
ISR(PCINT2_vect) {
  if (BUTTON_PRESSED_TIM >= TICKS_PER_SECOND) {
    BUTTON_PRESSED_TIM = 0;
    // Equivalent to: if (digitalRead(pin) == 0)
    if (!(PIND & ( 1 << SS_BUTTON ))) ss_button.is_pressed = true;
    if (!(PIND & ( 1 << OC_BUTTON ))) oc_button.is_pressed = true;
    if (!(PIND & ( 1 << EW_BUTTON ))) ew_button.is_pressed = true;
  }
}

// Interrupt Service Routine for PORT B -- Only EMG Button
ISR(PCINT0_vect) {
  // The emergency button is actually PB2
  if (!(PINB & ( 1 << 2 ))) emg_button.is_pressed = true; 
}


// Interrupt Service Routing for all of the timers and counters
// Used to control any non blocking delays
ISR(TIMER2_COMPB_vect){
	TCNT2 = 0;
  SYSTEM_COUNTER +=1;

  if (LED_COUNTER) {
    LED_COUNTER +=1;
    if (LED_COUNTER >= TICKS_PER_SECOND) {
      LED_COUNTER = 0;
      // Turn off the LEDS
      PORTD &= ~((1 << OC_LED) | (1 << EW_LED) | (1 << SS_LED));
    }
  }

  if (BUTTON_PRESSED_TIM <= 250) {
    BUTTON_PRESSED_TIM ++;
    
  }

  if (SYSTEM_COUNTER >= 61) {
    SYSTEM_TICK = 1;    
    SYSTEM_COUNTER = 0;
  }
}

void check_state() {

  if (SYSTEM_TICK) {
    SYSTEM_TICK = 0;
  }

  if (ss_button.is_pressed) {
    PORTD |= (1 << SS_LED);
    ss_button.is_pressed = false;
    ss_button.to_tx = true;
    LED_COUNTER +=1;

    switch (ss_button.cur_state) {
      case SS_NONE:
        ss_button.nx_state = B_START;
        #ifdef DEBUG
        Serial.println("SS -> START");
        #endif
        break;
      case B_STOP:
        ss_button.nx_state = B_START;
        #ifdef DEBUG
        Serial.println("SS -> START");
        #endif
        break;
      case B_START:
        ss_button.nx_state = B_STOP;
        #ifdef DEBUG
        Serial.println("SS -> STOP");
        #endif
        break;
      default:
        ss_button.nx_state = B_STOP;
        #ifdef DEBUG
        Serial.println("UNKNOWN STATE -- STOPPING");
        #endif
        break;
    }
  }   
  
  if (ew_button.is_pressed) {
    PORTD |= (1 << EW_LED); 
    ew_button.is_pressed = false;
    ew_button.to_tx = true;
    LED_COUNTER +=1;

    switch (ew_button.cur_state) {
      case EW_NONE:
        ew_button.nx_state = B_WEST;
        #ifdef DEBUG
        Serial.println("EW -> WEST");
        #endif
        break;
      case B_EAST:
        ew_button.nx_state = B_WEST;
        #ifdef DEBUG
        Serial.println("EW -> WEST");
        #endif
        break;
      case B_WEST:
        ew_button.nx_state = B_EAST;
        #ifdef DEBUG
        Serial.println("EW -> EAST");
        #endif
        break;
      default:
        ew_button.nx_state = B_WEST;
        #ifdef DEBUG
        Serial.println("UNKNOWN STATE -- WEST");
        #endif
        break;
    }
  }
  
  if (oc_button.is_pressed) {
    PORTD |= (1 << OC_LED); 
    oc_button.is_pressed = false;
    oc_button.to_tx = true;
    LED_COUNTER +=1;

    switch (oc_button.cur_state) {
      case OC_NONE:
        oc_button.nx_state = DOOR_OPEN;
        #ifdef DEBUG
        Serial.println("DOOR -> OPEN");
        #endif
        break;
      case DOOR_CLOSE:
        oc_button.nx_state = DOOR_OPEN;
        #ifdef DEBUG
        Serial.println("DOOR -> OPEN");
        #endif
        break;
      case DOOR_OPEN:
        oc_button.nx_state = DOOR_CLOSE;
        #ifdef DEBUG
        Serial.println("DOOR -> CLOSE");
        #endif
        break;
      default:
        oc_button.nx_state = DOOR_CLOSE;
        #ifdef DEBUG
        Serial.println("UNKNOWN STATE -- CLOSE");
        #endif
        break;
    }
  } 

  if (emg_button.is_pressed) {
    digitalWrite(EMG_LED, HIGH);
    emg_button.is_pressed = false;
    emg_button.to_tx = true;
    LED_COUNTER +=1;

    switch (emg_button.cur_state) {
      case B_NOT_EMERGENCY:
        emg_button.cur_state = B_EMERGENCY;
        #ifdef DEBUG
        Serial.println("EMERGENCY");
        #endif
        break;
      default:
        emg_button.cur_state = B_EMERGENCY;
        #ifdef DEBUG
        Serial.println("UNKNOWN STATE -- EMERGENCY");
        #endif
        break;
    }
  }
}

// Returns whether or not we should transmit
char next_state() {
  if (emg_button.cur_state == B_EMERGENCY) {
  // emergency();
  }

  // We may want to perform some analysis here
  // we can also slow down the state machine by adding a delay here
  if (ew_button.cur_state != ew_button.nx_state) {
    ew_button.cur_state = ew_button.nx_state;
  }

  if (ss_button.cur_state != ss_button.nx_state) {
    ss_button.cur_state = ss_button.nx_state;
  }

  if (oc_button.cur_state != oc_button.nx_state) {
    oc_button.cur_state = oc_button.nx_state;
  }

  return (oc_button.to_tx || ew_button.to_tx || ss_button.to_tx);
}

char transmit_state() {
  // TODO: Do some ACK/NACK between the master and slave
  message to_send = NONE;
  
  if (oc_button.to_tx) {
    if (oc_button.cur_state == DOOR_OPEN) to_send = OPEN;
    if (oc_button.cur_state == DOOR_CLOSE) to_send = CLOSE; 
  }

  if (ew_button.to_tx){
    if (ew_button.cur_state == B_EAST) to_send = EAST;
    if (ew_button.cur_state == B_WEST) to_send = WEST;
  }

  if (ss_button.to_tx){
    if (ss_button.cur_state == B_START) to_send = START;
    if (ss_button.cur_state == B_STOP) to_send = STOP;
  }

  return 1;
}

// Runs before every main loop to check if there is input from serial
void serialEvent(){
  comm_state = NOP;
  while (Serial.available()) {
    char in = Serial.read();
    comm_state = build_frame(&bt_i, in);
    Serial.write(in);      
  }

  switch(comm_state) {
    case TRANSMIT:
//      comm_state = transmit_frame(&bt_i);
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
  if (blue_serial.available() > 0) {
    blue_in = (message)blue_serial.read();
  }
  return blue_in;
}

void print_msg(message msg) {
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
