#ifndef PLATE_H
#define PLATE_H

#include "wheel.h"
#include "arm.h"

/* PINs */
/*
  !NOTE!
  Arduino pins that usable for intrrupts:
  Uno, Nano, Mini, other 328-based: 2, 3
  Mega, Mega2560, MegaADK: 2, 3, 18, 19, 20, 21
*/

#define FR_dig_pin_1 4
#define FR_dig_pin_2 5
#define FR_pwm_pin   6
#define FR_enc_pin_a 2
#define FR_enc_pin_b 7
#define FR_kp        2.5
#define FR_ki        0.8
#define FR_kd        0

#define FL_dig_pin_1 8
#define FL_dig_pin_2 9
#define FL_pwm_pin   10
#define FL_enc_pin_a 3
#define FL_enc_pin_b 11
#define FL_kp        2.5
#define FL_ki        0.8
#define FL_kd        0

#define BR_dig_pin_1 12
#define BR_dig_pin_2 13
#define BR_pwm_pin   14
#define BR_enc_pin_a 18
#define BR_enc_pin_b 15
#define BR_kp        2.5
#define BR_ki        0.8
#define BR_kd        0
#define ARM_pin_5

#define BL_dig_pin_1 16
#define BL_dig_pin_2 17
#define BL_pwm_pin   20
#define BL_enc_pin_a 19
#define BL_enc_pin_b 21
#define BL_kp        2.5
#define BL_ki        0.8
#define BL_kd        0

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

  long FR_enc_count;
  long FL_enc_count;
  long BR_enc_count;
  long BL_enc_count;

  Arm arm;

  void plate_get_enc_count();

public:
  Plate();
  ~Plate();

  /* Serial */
  int serial_input;
  int serial_buffer;

  /* GAME */
  // bool plate_before_take_ball();
  // bool plate_take_ball();
  // bool plate_release_ball();
  // void plate_manual();

  /* BASIC */
  void plate_connect();
  bool plate_command(Command command, int value);
  int plate_check_enc(WheelType wheel_type);
  void plate_rest_enc();
  void plate_get_serial_input();
};

#endif // !PLATE_H