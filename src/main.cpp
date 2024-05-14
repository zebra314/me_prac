#include <Arduino.h>
#include "plate.h"
#include "arm.h"
#include "blueTooth.cpp"

// BT param
int rcv = 0x04;
int vel = 0;

Plate plate(DEBUG::TEXT);
Arm arm;

void setup() {
  Serial.begin(9600);

  BlueTooth_setup();
  plate.plate_connect();

  arm.arm_connect();
  arm.arm_zero();

  Serial.println("Setup done");
}

bool executed = false;

// tmp
int servo_1_deg = 70;
int servo_2_deg = 180;

void loop() {

  // Game logic
  if (!executed) {
    // plate.plate_before_take_ball();
    // arm.arm_set_pos(ARM_POS::TAKE_BALL);
    // arm.arm_set_pos(ARM_POS::PRE_TAKE_BALL);
    // executed = true;
  }

  // Read the bluetooth signal and translate it to command
  if(BlueTooth_read(&rcv, &vel)){
    // Serial.print(rcv); Serial.print(" "); Serial.println(vel);
    switch (rcv)
    {
    case 16:
      plate.plate_command(Command::LINEAR_PWM, 120);
      break;

    case 17:
      plate.plate_command(Command::LINEAR_PWM, -120);
      break;

    case 18:
      plate.plate_command(Command::ANGULAR_PWM, 200);
      break;

    case 19:
      plate.plate_command(Command::ANGULAR_PWM, -200);
      break;

    case 20:
      plate.plate_command(Command::PAUSE, 0);
      break;

    case 33:
      servo_1_deg +=7;
      if (servo_1_deg > 180) servo_1_deg = 180;
      arm.arm_iso_ctrl(1, servo_1_deg);
      break;
    
    case 34:
      servo_1_deg -=7;
      if (servo_1_deg < 0) servo_1_deg = 0;
      arm.arm_iso_ctrl(1, servo_1_deg);
      break;
    
    case 35:
      servo_2_deg +=7;
      if (servo_2_deg > 180) servo_2_deg = 180;
      arm.arm_iso_ctrl(2, servo_2_deg);
      Serial.println(servo_2_deg);
      break;
    
    case 36:
      servo_2_deg -=7;
      if (servo_2_deg < 0) servo_2_deg = 0;
      arm.arm_iso_ctrl(2, servo_2_deg);
      Serial.println(servo_2_deg);
      break;
    case 37:
      arm.arm_set_pos(ARM_POS::ZERO);
      break;
    
    case 38:
      arm.arm_set_pos(ARM_POS::PRE_TAKE_BALL);
      break;
    
    case 39:
      arm.arm_set_pos(ARM_POS::TAKE_BALL);
      break;
    
    case 40:
      arm.arm_set_pos(ARM_POS::DROP_BALL);
      break;
    
    default:
      break;
    }
  }
}
