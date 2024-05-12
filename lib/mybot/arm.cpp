#include "arm.h"

Arm::Arm() {
  // Constructor
}

Arm::~Arm() {
  // Destructor
}

void Arm::arm_connect() {
  servo_1.servo.attach(SERVO_1_pin);
  servo_2.servo.attach(SERVO_2_pin);
  servo_3.servo.attach(SERVO_3_pin);
  servo_4.servo.attach(SERVO_4_pin);

  servo_1.upper_limit = SERVO_1_upper;
  servo_2.upper_limit = SERVO_2_upper;
  servo_3.upper_limit = SERVO_3_upper;
  servo_4.upper_limit = SERVO_4_upper;

  servo_1.lower_limit = SERVO_1_lower;
  servo_2.lower_limit = SERVO_2_lower;
  servo_3.lower_limit = SERVO_3_lower;
  servo_4.lower_limit = SERVO_4_lower;

  servo_1.ms_delay = 0;
  servo_2.ms_delay = 0;
  servo_3.ms_delay = 0;
  servo_4.ms_delay = 0;
}

void Arm::arm_zero() {
  arm_set_pos(ARM_POS::ZERO);
}

void Arm::arm_iso_ctrl(int servo_num, int deg) {
  switch (servo_num) {
  case 1:
    if (deg > this->servo_1.upper_limit || deg < this->servo_1.lower_limit) return; 
    this->servo_1.servo.write(deg);
    this->servo_1.current_deg = deg;
    break;

  case 2:
    if (deg > this->servo_2.upper_limit || deg < this->servo_2.lower_limit) return;
    this->servo_2.servo.write(deg);
    this->servo_2.current_deg = deg;
    break;

  case 3:
    if (deg > this->servo_3.upper_limit || deg < this->servo_3.lower_limit) return;
    this->servo_3.servo.write(deg);
    this->servo_3.current_deg = deg;
    break;

  case 4:
    if (deg > this->servo_4.upper_limit || deg < this->servo_4.lower_limit) return;
    this->servo_4.servo.write(deg);
    this->servo_4.current_deg = deg;
    break;
  }
}

void Arm::arm_set_pos(ARM_POS pos) {
  switch (pos) {
    case ARM_POS::ZERO:
      this->servo_1.target_deg = 90;
      this->servo_2.target_deg = 90;
      this->servo_3.target_deg = 90;
      this->servo_4.target_deg = 90;
      break;

    case ARM_POS::PRE_TAKE_BALL:
      this->servo_1.target_deg = 90;
      this->servo_2.target_deg = 90;
      this->servo_3.target_deg = 90;
      this->servo_4.target_deg = 90;
      break;

    case ARM_POS::TAKE_BALL:
      this->servo_1.target_deg = 90;
      this->servo_2.target_deg = 90;
      this->servo_3.target_deg = 90;
      this->servo_4.target_deg = 90;
      break;

    case ARM_POS::DROP_BALL:
      this->servo_1.target_deg = 90;
      this->servo_2.target_deg = 90;
      this->servo_3.target_deg = 90;
      this->servo_4.target_deg = 90;
      break;
    
    default:
      break;
  }
  arm_update_pos();
}

// TODO: Implement by FreeRTOS
void Arm::arm_update_pos() {
  if (servo_1.target_deg > servo_1.lower_limit && servo_1.target_deg  < servo_1.upper_limit) {
    servo_1.servo.write(servo_1.target_deg );
    servo_1.current_deg = servo_1.target_deg ;
  } 

  if (servo_2.target_deg > servo_2.lower_limit && servo_2.target_deg < servo_2.upper_limit) {
    servo_2.servo.write(servo_2.target_deg);
    servo_2.current_deg = servo_2.target_deg;
  }

  if (servo_3.target_deg > servo_2.lower_limit && servo_3.target_deg < servo_2.upper_limit) {
    servo_3.servo.write(servo_2.target_deg);
    servo_3.current_deg = servo_2.target_deg;
  }

  if (servo_4.target_deg > servo_2.lower_limit && servo_4.target_deg < servo_2.upper_limit) {
    servo_4.servo.write(servo_2.target_deg);
    servo_4.current_deg = servo_2.target_deg;
  }
}