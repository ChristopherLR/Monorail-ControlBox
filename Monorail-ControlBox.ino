#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "BTInterface.h"
#include <SoftwareSerial.h>

SoftwareSerial blue_serial(8, 9);
LiquidCrystal_I2C lcd(0x27, 20, 4);
bt_interface bt_i = {4, "INIT", &blue_serial};
char in = ' ';
comm_status comm_state = NOP;
#include "Helpers.h"

void setup() {
  Serial.begin(9600);

  //Initiate LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("S:    >     B: ");
  lcd.setCursor(0, 1);
  lcd.print("D:   M:        ");

  //Initiate Bluetooth module
  initialise_interface(&bt_i);
}

void loop() {
  message blue_in = read_msg();
  print_message(blue_in);
  display_message(blue_in);
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

/* First Line:   "S:    >     B: "
 * Second Line:  "D:   M:        "
 * S : Is used to indicate the current and next stations, CURR and NEXT will 
 * be replaced by EAST/WEST corresponding to the current and next stations
 * D : Is used to indicate the state of the door, this will either be OP = 
 * Open or CL = Close
 * M : Indicates the movement, this will be either ST = Start or SP = Stop
 * B : Will be used to indicate the most recent accepted button press, 
 * eg ST = Start, SP = Stop, E = East, W =  West, O = Open, C = Closed, 
 * EM = Emergency
 */
void display_message(message msg) {
  switch (msg) {
    case NONE:
      break;
    case EAST:
      lcd.setCursor(2, 0);
      lcd.print("EAST");
      lcd.setCursor(14, 0);
      lcd.print("E");
      break;
    case WEST:
      lcd.setCursor(2, 0);
      lcd.print("WEST");
      lcd.setCursor(14, 0);
      lcd.print("W");
      break;
    case START:
      lcd.setCursor(7, 1);
      lcd.print("ST");
      lcd.setCursor(14, 0);
      lcd.print("ST");
      break;
    case STOP:
      lcd.setCursor(7, 1);
      lcd.print("SP");
      lcd.setCursor(14, 0);
      lcd.print("SP");
      break;
    case OPEN:
      lcd.setCursor(2, 1);
      lcd.print("OP");
      lcd.setCursor(14, 0);
      lcd.print("OP");
      break;
    case CLOSE:
      lcd.setCursor(2, 1);
      lcd.print("CL");
      lcd.setCursor(14, 0);
      lcd.print("CL");
      break;
    case EMERGENCY:
      lcd.setCursor(14, 0);
      lcd.print("EM");
      lcd.setCursor(0, 0);
      lcd.print("Emergrency stop ");
      lcd.setCursor(0, 1);
      lcd.print("Please Reset");
      break;
    default:
      break;
  }
}
