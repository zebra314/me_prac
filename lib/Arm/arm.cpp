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

void Arm::arm_iso_ctrl(int servo_num, int deg, int ms_delay) {
  switch (servo_num) {
  case 1:
    base.ext_servo_set(deg, ms_delay);
    break;
  
  case 2:
    upper.ext_servo_set(deg, ms_delay);
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
    break;
  
  case ARM_POS::PRE_TAKE_BALL:
    this->base.target_deg = 0;
    this->upper.target_deg = 0;
    break;

  case ARM_POS::TAKE_BALL:
    this->base.target_deg = 0;
    this->upper.target_deg = 0;
    break;

  case ARM_POS::DROP_BALL:
    this->base.target_deg = 0;
    this->upper.target_deg = 0;
    break;
    
  default:
    break;
  }
  arm_update_pos();
}

void Arm::arm_update_pos() {
  base.ext_servo_set(base.target_deg, base.ms_delay);
  upper.ext_servo_set(upper.target_deg, upper.ms_delay);
}