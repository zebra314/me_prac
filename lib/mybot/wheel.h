#ifndef WHEEL_H
#define WHEEL_H

#include <Arduino.h>
#include <math.h>

#define ENABLE_ENCODER

#define PPR 11
#define GEAR_RATIO 50

class Wheel {
private:
  /* PINs */
  byte dc_pin_dig_1;
  byte dc_pin_dig_2;
  byte dc_pin_pwm;

  /* Encoder */
  volatile long encoder_count;
  int interrupt_num;

  /* RPMs */
  volatile long rpm_time_flag;
  volatile long encoder_count_flag;
  volatile long rpms;

  /* PID params*/
  long prev_time_pid;
  long prev_error;
  double error_integral;

  double kp;
  double ki;
  double kd;

public:
  Wheel();
  ~Wheel();
  
  void wheel_connect(byte dc_pin_dig_1, byte dc_pin_dig_2, byte dc_pin_pwm, 
                     byte encoder_pin_a, byte encoder_pin_b,
                     double kp, double ki, double kd);

  void wheel_pwm_ctrl(int pwm);
  void wheel_posi_ctrl(long posi);
  void wheel_rest_enc();
  double pid_control(long target);
  long get_encoder_count();
  long get_motor_rpms();

  /* Encoder items called by static function */
  byte encoder_pin_a;
  byte encoder_pin_b;
  void encoder_read();
};

#endif // !WHEEL_H