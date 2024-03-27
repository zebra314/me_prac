#include <Arduino.h>
#include "plate.h"

Plate plate(DEBUG::PLOT);

void setup() {
  plate.plate_connect();
}

void loop() {
  // plate.plate_command(Command::LINEAR_VEL, 0.4);
  plate.plate_command(Command::LINEAR_VEL, 0.7);

  plate.plate_command(Command::DEBUG, 0);
}
