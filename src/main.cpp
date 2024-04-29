#include <Arduino.h>
#include "plate.h"
#include "blueTooth.cpp"

// BT param
char rcv = 0x04;

Plate plate(DEBUG::TEXT);

void setup() {
  plate.plate_connect();
  BlueTooth_setup();
}

bool executed = false;

void loop() {
  // Read the bluetooth signal and translate it to command
  if(BlueTooth_read(&rcv)){
    switch (rcv)
    {
    case '0':
      plate.plate_command(Command::LINEAR_PWM, 120);
      break;

    case '1':
      plate.plate_command(Command::LINEAR_PWM, -120);
      break;

    case '2':
      plate.plate_command(Command::ANGULAR_PWM, 120);
      break;

    case '3':
      plate.plate_command(Command::ANGULAR_PWM, -120);
      break;

    case '4':
      plate.plate_command(Command::PAUSE, 0);
      break;

    default:
      break;
    }
  }

  if (!executed) {
    // plate.plate_before_take_ball();
    executed = true;
  }
}
