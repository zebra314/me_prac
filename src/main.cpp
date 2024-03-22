#include <Arduino.h>
#include <Wire.h>
#include <Plotter.h>
#include "wheel.h"
#include "arm.h"
#include "plate.h"

// #define ENABLE_PLOTTER

Plate plate;

/* PLOT */
#ifdef ENABLE_PLOTTER
Plotter plotter;
double plot_x;
#endif

void setup() {
  Serial.begin(9600);
  plate.plate_connect();
  
  #ifdef ENABLE_PLOTTER
  plotter.Begin();
  plotter.AddTimeGraph("Motor position", 5000, "time", plot_x);
  #endif
}

void loop() {
  plate.plate_command(Command::LINEAR_POSI, -2000);
  
  // Serial.println(plate.plate_check_enc(WheelType::FR));

  #ifdef ENABLE_PLOTTER
  plot_x = plate.plate_check_enc(WheelType::FR) * -1;
  plotter.Plot();
  #endif
}
