#include "arm.h"

Arm::Arm() {
  // Constructor
}

Arm::~Arm() {
  // Destructor
}

void Arm::arm_connect() {
  base.ext_servo_connect(SERVO_base_pin, SERVO_base_init, SERVO_base_min, SERVO_base_max);
  upper.ext_servo_connect(SERVO_upper_pin, SERVO_upper_init, SERVO_upper_min, SERVO_upper_max);
}

void Arm::arm_zero() {
  base.ext_servo_zero();
  upper.ext_servo_zero();
}

void Arm::arm_mv_target(int servo_num, int deg, int ms_delay = 0, int mv_unit = 1) {
  switch (servo_num) {
  case 1:
    base.target_deg = deg;
    base.ms_delay = ms_delay;
    base.mv_unit = mv_unit;
    break;
  
  case 2:
    upper.target_deg = deg;
    upper.ms_delay = ms_delay;
    upper.mv_unit = mv_unit;
    break;
  
  default:
    break;
  }
}

void Arm::arm_mv_delta(int servo_num, int delta_deg, int ms_delay = 0, int mv_unit = 1) {
  switch (servo_num) {
  case 1:
    base.target_deg += delta_deg;
    base.ms_delay = ms_delay;
    base.mv_unit = mv_unit;
    break;
  
  case 2:
    upper.target_deg += delta_deg;
    upper.ms_delay = ms_delay;
    upper.mv_unit = mv_unit;
    break;
  
  default:
    break;
  }
}

void Arm::arm_set_pos(ARM_POS pos) {
  switch (pos) {
  case ARM_POS::ZERO:
    this->base.target_deg = 90;
    this->upper.target_deg = 90;

    this->base.ms_delay = 17;
    this->upper.ms_delay = 17;

    this->base.mv_unit = 3;
    this->upper.mv_unit = 3;
    break;
  
  case ARM_POS::PRE_TAKE_BALL:
    this->base.target_deg = 30;
    this->upper.target_deg = 30;

    this->base.ms_delay = 17;
    this->upper.ms_delay = 17;

    this->base.mv_unit = 3;
    this->upper.mv_unit = 3;
    break;

  case ARM_POS::TAKE_BALL:
    this->base.target_deg = 12;
    this->upper.target_deg = 18;

    this->base.ms_delay = 17;
    this->upper.ms_delay = 17;

    this->base.mv_unit = 4;
    this->upper.mv_unit = 4;
    break;

  case ARM_POS::DROP_BALL:
    this->base.target_deg = 35;
    this->upper.target_deg = 160;

    this->base.ms_delay = 17;
    this->upper.ms_delay = 17;

    this->base.mv_unit = 3;
    this->upper.mv_unit = 3;
    break;
  
  case ARM_POS::FREEZE:
    this->base.target_deg = this->base.current_deg;
    this->upper.target_deg = this->upper.current_deg;

    this->base.ms_delay = 0;
    this->upper.ms_delay = 0;

    this->base.mv_unit = 0;
    this->upper.mv_unit = 0;
    break;
    
  default:
    break;
  }
}