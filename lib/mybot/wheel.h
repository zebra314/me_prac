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
  volatile double encoder_count_flag;
  volatile double vel;

  /* PID params*/
  double prev_pos_time;
  double prev_pos_error;
  double pos_error_integral;

  double pos_kp;
  double pos_ki;
  double pos_kd;

  double prev_vel_time;
  double prev_vel_error;
  double vel_error_integral;

  double vel_kp;
  double vel_ki;
  double vel_kd;

public:
  Wheel();
  ~Wheel();
  
  void wheel_connect(byte dc_pin_dig_1, byte dc_pin_dig_2, byte dc_pin_pwm, 
                     byte encoder_pin_a, byte encoder_pin_b,
                     double pos_kp, double pos_ki, double pos_kd,
                     double vel_kp, double vel_ki, double vel_kd);

  void wheel_pwm_ctrl(double pwm);
  void wheel_pos_ctrl(double pos);
  void wheel_vel_ctrl(double vel);
  void wheel_comp_ctrl(double vel, double pos);

  double pos_pid(double target);
  double vel_pid(double target);
  double comp_pid(double target_vel, double target_pos);

  void wheel_rest_enc();
  double get_encoder_count();
  double get_motor_vel();

  /* Encoder items called by static function */
  byte encoder_pin_a;
  byte encoder_pin_b;
  void encoder_read();
};

#endif // !WHEEL_H