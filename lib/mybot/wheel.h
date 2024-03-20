#ifndef WHEEL_H
#define WHEEL_H

#include <Arduino.h>
#include <math.h>

// We set the encoder intrupt in the setup function of the main file
// #define ENABLE_ENCODER

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

  friend void encoder_read();

public:
  Wheel();
  ~Wheel();
  
  void wheel_connect(byte dc_pin_dig_1, byte dc_pin_dig_2, byte dc_pin_pwm, byte encoder_pin_a, byte encoder_pin_b);
  void wheel_move(int pwm);
  float pid_control(int target);
  
  volatile long get_encoder_count();
};

#endif // !WHEEL_H