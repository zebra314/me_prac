#ifndef ARM_H
#define ARM_H

#include <Arduino.h>
#include <Servo.h>

class Arm {
private:
  byte servo_plate_pin;
  byte servo_2_pin;
  byte servo_3_pin;
  byte servo_4_pin;
  byte servo_5_pin;
  byte servo_gripper_pin;

  Servo servo_plate;
  Servo servo_2;
  Servo servo_3;
  Servo servo_4;
  Servo servo_5;
  Servo servo_gripper;

public:
  Arm();
  ~Arm();

  void arm_connect(byte servo_plate_pin, byte servo_2_pin, byte servo_3_pin, byte servo_4_pin, byte servo_5_pin, byte servo_gripper_pin);
  void arm_iso_ctrl(int plate_num, int deg);
};



#endif // !ARM_H