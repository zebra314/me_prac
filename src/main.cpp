#include <Arduino.h>
#include <Wire.h>
#include <Plotter.h>
#include "wheel.h"
#include "arm.h"
#include "plate.h"

#define ENABLE_PLOTTER

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
}

void loop() {
  plate.plate_command(Command::LINEAR_VEL, 0.4);
  // plate.plate_command(Command::LINEAR_VEL, 0.7);
  
  #ifndef ENABLE_PLOTTER
  // Serial.print("FL: ");
  Serial.println(plate.plate_check_enc(WheelType::FL));
  // Serial.print(" BL: ");
  // Serial.println(plate.plate_check_enc(WheelType::BL));
  #endif

  #ifdef ENABLE_PLOTTER
  FL_motor_posi = plate.plate_check_enc(WheelType::FL) * -1;
  BL_motor_posi = plate.plate_check_enc(WheelType::BL) * -1;
  plotter.Plot();
  #endif
}
