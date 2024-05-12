#include "arm.h"

Arm::Arm() {
  // Constructor
}

Arm::~Arm() {
  // Destructor
}

void Arm::arm_connect(byte servo_0_pin, byte servo_1_pin, byte servo_2_pin, byte servo_3_pin) {
  this->servo_0_pin = servo_0_pin;
  this->servo_1_pin = servo_1_pin;
  this->servo_2_pin = servo_2_pin;
  this->servo_3_pin = servo_3_pin;

  this->servo_0.attach(servo_0_pin);
  this->servo_1.attach(servo_1_pin);
  this->servo_2.attach(servo_2_pin);
  this->servo_3.attach(servo_3_pin);
}

void Arm::arm_iso_ctrl(int plate_num, int deg){
  switch (plate_num) {
    case 0:
      this->servo_0.write(deg);
      break;
    case 1:
      this->servo_1.write(deg);
      break;
    case 2:
      this->servo_2.write(deg);
      break;
    case 3:
      this->servo_3.write(deg);
      break;
  }
}
