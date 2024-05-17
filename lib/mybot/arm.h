#ifndef ARM_H
#define ARM_H

#include <Arduino.h>
#include <Servo.h>
#include "ext_servo.h"
#include "settings.h"

enum class ARM_POS:char {
  ZERO,
  PRE_TAKE_BALL,
  TAKE_BALL,
  DROP_BALL,
};

class Arm {
private:
  ExtServo base_r;
  ExtServo base_l;
  ExtServo upper;

  void arm_update_pos();

public:
  Arm();
  ~Arm();

  void arm_connect();
  void arm_zero();
  void arm_iso_ctrl(int servo_num, int deg, int ms_delay = 0);
  void arm_set_pos(ARM_POS pos);
};

#endif // !ARM_H