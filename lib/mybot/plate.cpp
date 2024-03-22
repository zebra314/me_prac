#include "plate.h"

Plate::Plate() {
  // Constructor
  Wheel FR;
  Wheel FL;
  Wheel BR;
  Wheel BL;
  Arm arm;

  this->FR = FR;
  this->FL = FL;
  this->BR = BR;
  this->BL = BL;
  this->arm = arm;
}

Plate::~Plate() {
  // Destructor
  FR.~Wheel();
  FL.~Wheel();
  BR.~Wheel();
  BL.~Wheel();
}

void Plate::plate_connect() {
  FR.wheel_connect(FR_dig_pin_1, FR_dig_pin_2, FR_pwm_pin, FR_enc_pin_a, FR_enc_pin_b, FR_kp, FR_ki, FR_kd);
  FL.wheel_connect(FL_dig_pin_1, FL_dig_pin_2, FL_pwm_pin, FL_enc_pin_a, FL_enc_pin_b, FL_kp, FL_ki, FL_kd);
  BR.wheel_connect(BR_dig_pin_1, BR_dig_pin_2, BR_pwm_pin, BR_enc_pin_a, BR_enc_pin_b, BR_kp, BR_ki, BR_kd);
  BL.wheel_connect(BL_dig_pin_1, BL_dig_pin_2, BL_pwm_pin, BL_enc_pin_a, BL_enc_pin_b, BL_kp, BL_ki, BL_kd);
  arm.arm_connect(ARM_pin_0, ARM_pin_1, ARM_pin_2, ARM_pin_3, ARM_pin_4, ARM_pin_gripper);
}

void Plate::plate_get_serial_input() {
  char tmp;
  if (Serial.available() > 0) {
    tmp = Serial.read();
    if (tmp == '\n') {
      Serial.println(String(this->serial_buffer));
      this->serial_input = this->serial_buffer;
      this->serial_buffer = 0;
    } else if (tmp >= '0' && tmp <= '9') {
      this->serial_buffer = this->serial_buffer * 10 + (tmp - '0');
    }
  }
}

/* Encoder pulse output per revolution 
  = Basic pulse x gear ratio

  ChiHai Motor CHR-GM37 1:50, with 11PPR encoder
  = 11 x 50 = 550

  pulse per revolution = 550
  pulse per meter = 550 / (wheel diameter * PI)
*/

bool done = false;
bool not_done = true;
bool Plate::plate_command(Command command, int value) {
  switch (command) {
    case Command::LINEAR_POSI:
      FR.wheel_posi_ctrl(value);
      FL.wheel_posi_ctrl(value);
      BR.wheel_posi_ctrl(value);
      BL.wheel_posi_ctrl(value);
      break;
    case Command::LINEAR_VEL:
      FR.wheel_pwm_ctrl(value);
      FL.wheel_pwm_ctrl(value);
      BR.wheel_pwm_ctrl(value);
      BL.wheel_pwm_ctrl(value);
      break;
    case Command::ANGULAR_POSI:
      FR.wheel_posi_ctrl(value);
      FL.wheel_posi_ctrl(-value);
      BR.wheel_posi_ctrl(-value);
      BL.wheel_posi_ctrl(value);
      break;
    case Command::ANGULAR_VEL:
      FR.wheel_pwm_ctrl(value);
      FL.wheel_pwm_ctrl(-value);
      BR.wheel_pwm_ctrl(-value);
      BL.wheel_pwm_ctrl(value);
      break;
    case Command::PAUSE:
      FR.wheel_pwm_ctrl(0);
      FL.wheel_pwm_ctrl(0);
      BR.wheel_pwm_ctrl(0);
      BL.wheel_pwm_ctrl(0);
      break;
    case Command::RECORD:
      FR.wheel_pwm_ctrl(0);
      FL.wheel_pwm_ctrl(0);
      BR.wheel_pwm_ctrl(0);
      BL.wheel_pwm_ctrl(0);
      break;
  }
  return done;
}

// bool Plate::plate_before_take_ball(){

// }

// bool Plate::plate_take_ball(){

// }

// bool Plate::plate_release_ball(){

// }

int Plate::plate_check_enc(WheelType wheel_type) {
  switch (wheel_type) {
    case WheelType::FR:
      return FR_enc_count;
      break;
    case WheelType::FL:
      return FL_enc_count;
      break;
    case WheelType::BR:
      return BR_enc_count;
      break;
    case WheelType::BL:
      return BL_enc_count;
      break;
  }
}

void Plate::plate_rest_enc() {
  noInterrupts();
  FR.wheel_rest_enc();
  FL.wheel_rest_enc();
  BR.wheel_rest_enc();
  BL.wheel_rest_enc();
  interrupts();
}

void Plate::plate_get_enc_count() {
  noInterrupts();
  FR_enc_count = FR.get_encoder_count();
  FL_enc_count = FL.get_encoder_count();
  BR_enc_count = BR.get_encoder_count();
  BL_enc_count = BL.get_encoder_count();
  interrupts();
}
