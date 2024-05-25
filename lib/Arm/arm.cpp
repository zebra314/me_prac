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

void Arm::arm_mv_target(int servo_num, int deg, int ms_delay) {
  switch (servo_num) {
  case 1:
    base.target_deg = deg;
    base.ms_delay = ms_delay;
    break;
  
  case 2:
    upper.target_deg = deg;
    upper.ms_delay = ms_delay;
    break;
  
  default:
    break;
  }
}

void Arm::arm_mv_delta(int servo_num, int delta_deg, int ms_delay) {
  switch (servo_num) {
  case 1:
    base.target_deg += delta_deg;
    base.ms_delay = ms_delay;
    break;
  
  case 2:
    upper.target_deg += delta_deg;
    upper.ms_delay = ms_delay;
    break;
  
  default:
    break;
  }
}

void Arm::arm_set_pos(ARM_POS pos) {
  switch (pos) {
  case ARM_POS::ZERO:
    this->base.target_deg = 0;
    this->upper.target_deg = 0;

    this->base.ms_delay = 70;
    this->upper.ms_delay = 70;
    break;
  
  case ARM_POS::PRE_TAKE_BALL:
    this->base.target_deg = 0;
    this->upper.target_deg = 0;

    this->base.ms_delay = 70;
    this->upper.ms_delay = 70;
    break;

  case ARM_POS::TAKE_BALL:
    this->base.target_deg = 0;
    this->upper.target_deg = 0;

    this->base.ms_delay = 70;
    this->upper.ms_delay = 70;
    break;

  case ARM_POS::DROP_BALL:
    this->base.target_deg = 0;
    this->upper.target_deg = 0;

    this->base.ms_delay = 70;
    this->upper.ms_delay = 70;
    break;
    
  default:
    break;
  }
}