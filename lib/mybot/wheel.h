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
  int interrupt_num;
  volatile double encoder_count;
  volatile double vel_time_flag;

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
                     double pos_kp, double pos_ki, double pos_kd,
                     double vel_kp, double vel_ki, double vel_kd);

  void wheel_pwm_ctrl(int pwm);
  void wheel_posi_ctrl(long posi);
  void wheel_rest_enc();
  double get_encoder_count();
  double get_motor_vel();

  /* Encoder items called by static function */
  byte encoder_pin_a;
  byte encoder_pin_b;
  void encoder_read();
};

#endif // !WHEEL_H