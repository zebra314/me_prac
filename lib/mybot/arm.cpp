#include "arm.h"

Arm::Arm() {
  // Constructor
}

Arm::~Arm() {
  // Destructor
}

void Arm::arm_connect(byte servo_plate_pin, byte servo_2_pin, byte servo_3_pin, byte servo_4_pin, byte servo_5_pin, byte servo_gripper_pin) {
  this->servo_plate_pin = servo_plate_pin;
  this->servo_2_pin = servo_2_pin;
  this->servo_3_pin = servo_3_pin;
  this->servo_4_pin = servo_4_pin;
  this->servo_5_pin = servo_5_pin;
  this->servo_gripper_pin = servo_gripper_pin;

  this->servo_plate.attach(servo_plate_pin);
  this->servo_2.attach(servo_2_pin);
  this->servo_3.attach(servo_3_pin);
  this->servo_4.attach(servo_4_pin);
  this->servo_5.attach(servo_5_pin);
  this->servo_gripper.attach(servo_gripper_pin);
}

void Arm::arm_iso_ctrl(int plate_num, int deg){
  switch (plate_num) {
    case 1:
      this->servo_plate.write(deg);
      break;
    case 2:
      this->servo_2.write(deg);
      break;
    case 3:
      this->servo_3.write(deg);
      break;
    case 4:
      this->servo_4.write(deg);
      break;
    case 5:
      this->servo_5.write(deg);
      break;
    case 6:
      this->servo_gripper.write(deg);
      break;
  }
}
