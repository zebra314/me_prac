#include <Arduino.h>
#include "plate.h"
#include "blueTooth.h"

#define ENABLE_PLOTTER

// these are bluetooth related code
char rcv;
Command cmd;

Plate plate;

/* PLOT */
#ifdef ENABLE_PLOTTER
Plotter plotter;
double FL_motor_posi, BL_motor_posi;
#endif

void setup() {
  #ifndef ENABLE_PLOTTER 
  Serial.begin(9600);
  #endif
  plate.plate_connect();
  
  #ifdef ENABLE_PLOTTER
  plotter.Begin();
  plotter.AddTimeGraph("Motor position", 5000, "FL", FL_motor_posi, "BL", BL_motor_posi);
  #endif

  // bluetooth setup
  BlueTooth_setup();

}

void loop() {

  // read the bluetooth signal and translate it to command
  BlueTooth_read(&rcv);
  cmd = (*rcv) >> 4;

  #ifndef ENABLE_PLOTTER
  Serial.print("BlueTooth receive: %d\n", *rcv);
  #endif


  plate.plate_command(Command::LINEAR_VEL, 0.4);
  // plate.plate_command(Command::LINEAR_VEL, 0.7);
  
  #ifndef ENABLE_PLOTTER
  // Serial.print("FL: ");
  Serial.println(plate.plate_check_enc(WheelType::FL));
  // Serial.print(" BL: ");
  // Serial.println(plate.plate_check_enc(WheelType::BL));
  #endif

  plate.plate_command(Command::DEBUG, 0);
}
