#include <Arduino.h>
#include "plate.h"
#include "blueTooth.cpp"

// these are bluetooth related code
char rcv;
Command cmd;

Plate plate;

void setup() {
  plate.plate_connect();
  BlueTooth_setup();
}

void loop() {

  // read the bluetooth signal and translate it to command
  BlueTooth_read(&rcv);
  
  switch (rcv)
  {
  case FORWARD:
    plate.plate_command(Command::LINEAR_VEL, 0.4);
    break;

  case BACKWARD:
    plate.plate_command(Command::LINEAR_VEL, -0.4);
    break;

  case CLOCKWISE:
    plate.plate_command(Command::ANGULAR_VEL, 0.4);
    break;

  case COUNTER_CLOCKWISE:
    plate.plate_command(Command::ANGULAR_VEL, -0.4);
    break;

  default:
    break;
  }

  // plate.plate_command(Command::DEBUG, 0);
}
