#ifndef PLATE_H
#define PLATE_H

#include "wheel.h"
#include "arm.h"
#include <Plotter.h>

#define ENABLE_PLOTTER

/* PINs */

/*
  Arduino pins that usable for intrrupts:
  Uno, Nano, Mini, other 328-based: 2, 3
  Mega, Mega2560, MegaADK: 2, 3, 18, 19, 20, 21
*/

#define FR_dig_pin_1 4
#define FR_dig_pin_2 5
#define FR_pwm_pin   6
#define FR_enc_pin_a 2
#define FR_enc_pin_b 7
#define FR_kp        2.7
#define FR_ki        0.04
#define FR_kd        0.8

#define FL_dig_pin_1 8
#define FL_dig_pin_2 9
#define FL_pwm_pin   10
#define FL_enc_pin_a 3
#define FL_enc_pin_b 11
#define FL_kp        2.7
#define FL_ki        0.04
#define FL_kd        0.8

#define BR_dig_pin_1 12
#define BR_dig_pin_2 13
#define BR_pwm_pin   14
#define BR_enc_pin_a 18
#define BR_enc_pin_b 15
#define BR_kp        2.7
#define BR_ki        0.04
#define BR_kd        0.8

#define BL_dig_pin_1 36
#define BL_dig_pin_2 38
#define BL_pwm_pin   20
#define BL_enc_pin_a 19
#define BL_enc_pin_b 21
#define BL_kp        2.7
#define BL_ki        0.04
#define BL_kd        0.8

#define ARM_pin_0    22
#define ARM_pin_1    23
#define ARM_pin_2    24
#define ARM_pin_3    25
#define ARM_pin_4    26
#define ARM_pin_gripper 27

#define PPR 11
#define GEAR_RATIO 50
#define WHEEL_DIAMETER 0.1

enum class Command:char {
  LINEAR_POSI,
  LINEAR_VEL,
  ANGULAR_POSI,
  ANGULAR_VEL,
  PAUSE,
  RECORD,
  DEBUG,
};

enum class DEBUG:char {
  PLOT,
  TEXT,
  NONE,
};

enum class WheelType:char {
  FR,
  FL,
  BR,
  BL,
};

class Plate {
private:
  Wheel FR;
  Wheel FL;
  Wheel BR;
  Wheel BL;
  Arm arm;

  long FR_enc_count;
  long FL_enc_count;
  long BR_enc_count;
  long BL_enc_count;

  long FR_rpms;
  long FL_rpms;
  long BR_rpms;
  long BL_rpms;

  int FR_target;
  int FL_target;
  int BR_target;
  int BL_target;

  long current_time;
  long previous_time;
  long print_time_flag;

  Plotter plotter;

  DEBUG debug;

  void plate_check_info();
  void plate_update_state();
  void plate_move();

public:
  Plate(DEBUG debug);
  ~Plate();

  int serial_input;
  int serial_buffer;

  /* GAME */
  // bool plate_before_take_ball();
  // bool plate_take_ball();
  // bool plate_release_ball();
  // void plate_manual();

  /* BASIC */
  void plate_connect();
  bool plate_command(Command command, float value);
  void plate_rest_enc();
  void plate_get_serial_input();
};

#endif // !PLATE_H