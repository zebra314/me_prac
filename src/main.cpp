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
  cmd = (*rcv) >> 4;


  plate.plate_command(Command::LINEAR_VEL, 0.4);
  plate.plate_command(Command::DEBUG, 0);
}
