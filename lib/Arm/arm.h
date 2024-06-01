#ifndef ARM_H
#define ARM_H

#include <Arduino.h>
#include "ext_servo.h"
#include "settings.h"

enum class ARM_POS:char {
  ZERO,
  PRE_TAKE_BALL,
  TAKE_BALL,
  DROP_BALL,
  FREEZE,
};

class Arm {
private:
  ExtServo base;
  ExtServo upper;

public:
  Arm();
  ~Arm();

  void arm_connect();
  void arm_zero();
  void arm_mv_target(int servo_num, int target_deg, int ms_delay = 0, int mv_unit = 1);
  void arm_mv_delta(int servo_num, int delta_deg, int ms_delay = 0, int mv_unit = 1);
  void arm_set_pos(ARM_POS pos);
};

#endif // !ARM_H