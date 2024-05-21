#include "arm.h"

Arm::Arm() {
  // Constructor
}

Arm::~Arm() {
  // Destructor
}

void Arm::arm_connect() {
  base_l.ext_servo_connect(SERVO_base_l_pin, SERVO_base_l_init, SERVO_base_l_min, SERVO_base_l_max);
  base_r.ext_servo_connect(SERVO_base_r_pin, SERVO_base_r_init, SERVO_base_r_min, SERVO_base_r_max);
  upper.ext_servo_connect(SERVO_upper_pin, SERVO_upper_init, SERVO_upper_min, SERVO_upper_max);
}

void Arm::arm_zero() {
  base_l.ext_servo_zero();
  base_r.ext_servo_zero();
  upper.ext_servo_zero();
}

void Arm::arm_iso_ctrl(int servo_num, int deg, int ms_delay) {
  switch (servo_num) {
  case 1:
    base_l.ext_servo_set(deg, ms_delay);
    break;
  
  case 2:
    base_r.ext_servo_set(deg, ms_delay);
    break;
  
  case 3:
    upper.ext_servo_set(deg, ms_delay);
    break;
  
  default:
    break;
  }

}

void Arm::arm_set_pos(ARM_POS pos) {
  switch (pos) {
  case ARM_POS::ZERO:
    this->base_l.target_deg = 0;
    this->base_r.target_deg = 0;
    this->upper.target_deg = 0;
    break;
  
  case ARM_POS::PRE_TAKE_BALL:
    this->base_l.target_deg = 0;
    this->base_r.target_deg = 0;
    this->upper.target_deg = 0;
    break;

  case ARM_POS::TAKE_BALL:
    this->base_l.target_deg = 0;
    this->base_r.target_deg = 0;
    this->upper.target_deg = 0;
    break;

  case ARM_POS::DROP_BALL:
    this->base_l.target_deg = 0;
    this->base_r.target_deg = 0;
    this->upper.target_deg = 0;
    break;
    
  default:
    break;
  }
  arm_update_pos();
}

void Arm::arm_update_pos() {
  base_l.ext_servo_set(base_l.target_deg, 0);
  base_r.ext_servo_set(base_r.target_deg, 0);
  upper.ext_servo_set(upper.target_deg, 0);
}