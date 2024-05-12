#ifndef ARM_H
#define ARM_H

#include <Arduino.h>
#include <Servo.h>

class Arm {
private:
  byte servo_0_pin;
  byte servo_1_pin;
  byte servo_2_pin;
  byte servo_3_pin;
  byte servo_4_pin;
  byte servo_gripper_pin;

  Servo servo_0;
  Servo servo_1;
  Servo servo_2;
  Servo servo_3;
  Servo servo_4;
  Servo servo_gripper;

public:
  Arm();
  ~Arm();

  void arm_connect(byte servo_0_pin, byte servo_1_pin, byte servo_2_pin, byte servo_3_pin);
  void arm_iso_ctrl(int plate_num, int deg);
};



#endif // !ARM_H