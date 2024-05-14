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

  servo_1.upper_limit = SERVO_1_upper;
  servo_2.upper_limit = SERVO_2_upper;

  servo_1.lower_limit = SERVO_1_lower;
  servo_2.lower_limit = SERVO_2_lower;

  servo_1.ms_delay = 0;
  servo_2.ms_delay = 0;
}

void Arm::arm_zero() {
  servo_1.servo.write(70);
  servo_2.servo.write(180);

  servo_1.current_deg = 70;
  servo_2.current_deg = 180;
}

void Arm::arm_iso_ctrl(int servo_num, int deg) {
  if(deg > 180) deg = 180;
  if(deg < 0) deg = 0;
  
  switch (servo_num) {
  case 1:
    this->servo_1.servo.write(deg);
    this->servo_1.current_deg = deg;
    break;

  case 2:
    this->servo_2.servo.write(deg);
    this->servo_2.current_deg = deg;
    break;

  default:
    break;
  }
}


void Arm::arm_set_pos(ARM_POS pos) {
  switch (pos) {
  case ARM_POS::ZERO:
    this->servo_1.target_deg = 70;
    this->servo_2.target_deg = 180;
    break;
  
  case ARM_POS::PRE_TAKE_BALL:
    this->servo_1.target_deg = 135;
    this->servo_2.target_deg = 80;
    break;

  case ARM_POS::TAKE_BALL:
    this->servo_1.target_deg = 175;
    this->servo_2.target_deg = 30;
    break;

  case ARM_POS::DROP_BALL:
    this->servo_1.target_deg = 100;
    this->servo_2.target_deg = 0;
    break;
    
  default:
    break;
  }
  arm_update_pos();
}

// TODO: Implement by FreeRTOS
void Arm::arm_update_pos() {
  while(true) {
    if (servo_1.target_deg == servo_1.current_deg && servo_2.target_deg == servo_2.current_deg) {
      break;
    }
    // Serial.print(servo_1.current_deg);Serial.print(" ");Serial.println(servo_2.current_deg);

    if (servo_1.target_deg > servo_1.current_deg) {
      servo_1.current_deg++;
    } else if (servo_1.target_deg < servo_1.current_deg) {
      servo_1.current_deg--;
    }

    if (servo_2.target_deg > servo_2.current_deg) {
      servo_2.current_deg++;
    } else if (servo_2.target_deg < servo_2.current_deg) {
      servo_2.current_deg--;
    }

    servo_1.servo.write(servo_1.current_deg);
    servo_2.servo.write(servo_2.current_deg);

    for(int i = 0; i < 300; i++) {
    }
  // if (servo_1.target_deg >= servo_1.lower_limit && servo_1.target_deg  <= servo_1.upper_limit) {
  //   servo_1.servo.write(servo_1.target_deg );
  //   servo_1.current_deg = servo_1.target_deg ;
  // } 

  // if (servo_2.target_deg >= servo_2.lower_limit && servo_2.target_deg <= servo_2.upper_limit) {
  //   servo_2.servo.write(servo_2.target_deg);
  //   servo_2.current_deg = servo_2.target_deg;
  // }
  }
}