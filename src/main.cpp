#include <Arduino.h>
#include <Wire.h>
#include <Plotter.h>
#include "wheel.h"
#include "arm.h"

// #define ENABLE_PLOTTER

/* PINs */
const byte ENCA = 2;
const byte ENCB = 4;
const byte IN3 = 7;
const byte IN4 = 6;
const byte SPD = 5;

/* ROBOT COMPONENTs */
Wheel wheel;
Arm arm;

/* USER INPUT */
int inputValue = 0;
int buffer;

/* FUNCTIONS */
void get_serial_input();

/* PLOT */
#ifdef ENABLE_PLOTTER
Plotter plotter;
double plot_x;
#endif

void setup() {
  Serial.begin(9600);
  wheel.wheel_connect(IN4, IN3, SPD, ENCA, ENCB);
  // arm.arm_connect(8,9,10,11,12,13);

  #ifdef ENABLE_PLOTTER
  plotter.Begin();
  plotter.AddTimeGraph("Motor position", 5000, "time", plot_x);
  #endif
}

void loop() {
  // int target = -2000;
  // int pwm = wheel.pid_control(target);
  // wheel.wheel_move(pwm);
  
  Serial.println(wheel.get_encoder_count());

  #ifdef ENABLE_PLOTTER
  plot_x = wheel.get_encoder_count()* -1;
  plotter.Plot();
  #endif
}

void get_serial_input() {
  char tmp;
  if (Serial.available() > 0) {
    tmp = Serial.read();
    if (tmp == '\n') {
      Serial.println(String(buffer));
      inputValue = buffer;
      buffer = 0;
    } else if (tmp >= '0' && tmp <= '9') {
      buffer = buffer * 10 + (tmp - '0');
    }
  }
}
