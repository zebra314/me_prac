#ifndef ARM_H
#define ARM_H

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <Servo.h>
#include "settings.h"

enum class ARM_POS:char {
  ZERO,
  PRE_TAKE_BALL,
  TAKE_BALL,
  DROP_BALL,
};

struct ext_servo {
  Servo servo;
  int target_deg;
  int current_deg;
  int ms_delay;
  int upper_limit;
  int lower_limit;
};

class Arm {
private:
  ext_servo servo_1;
  ext_servo servo_2;

  void arm_update_pos();

public:
  Arm();
  ~Arm();

  void arm_connect();
  void arm_zero();
  void arm_iso_ctrl(int servo_num, int deg);
  void arm_set_pos(ARM_POS pos);
};

#endif // !ARM_H