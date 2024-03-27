#ifndef WHEEL_H
#define WHEEL_H

#include <Arduino.h>
#include <math.h>

#define ENABLE_ENCODER

class Wheel {
private:
  /* PINs */
  byte dc_pin_dig_1;
  byte dc_pin_dig_2;
  byte dc_pin_pwm;

  /* Encoder */
  volatile long encoder_count;
  int interrupt_num;

  /* PID params*/
  long prev_time;
  float prev_error;
  float error_integral;

  float kp;
  float ki;
  float kd;

public:
  Wheel();
  ~Wheel();
  
  void wheel_connect(byte dc_pin_dig_1, byte dc_pin_dig_2, byte dc_pin_pwm, 
                     byte encoder_pin_a, byte encoder_pin_b,
                     float kp, float ki, float kd);

  void wheel_pwm_ctrl(int pwm);
  void wheel_posi_ctrl(int posi);
  void wheel_rest_enc();
  float pid_control(int target);
  long get_encoder_count();

  /* Encoder items called by static function */
  byte encoder_pin_a;
  byte encoder_pin_b;
  void encoder_read();

};

#endif // !WHEEL_H