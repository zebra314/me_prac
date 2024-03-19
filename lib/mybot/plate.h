#ifndef PLATE_H
#define PLATE_H

#include <Arduino.h>
#include <math.h>

class Wheel {
private:
  byte dc_pin_dig_1;
  byte dc_pin_dig_2;
  byte dc_pin_pwm;

  byte encoder_pin_a;
  byte encoder_pin_b;

  volatile long encoder_count;

  long prev_time;
  float prev_error;
  float error_integral;

  float kp;
  float ki;
  float kd;

  static Wheel *wheel_instance;
  static void encoder_isr();

  void encoder_read();

public:
  Wheel();
  ~Wheel();

  void wheel_connect(byte dc_pin_dig_1, byte dc_pin_dig_2, byte dc_pin_pwm, byte encoder_pin_a, byte encoder_pin_b);
  void wheel_move(int pwm);
  float pid_control(int target);
  volatile long get_encoder_count();
};

class Plate {
private:
  Wheel FR;
  Wheel FL;
  Wheel BR;
  Wheel BL;
};

#endif // !PLATE_H