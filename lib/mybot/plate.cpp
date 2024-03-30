#include "plate.h"
#include "wheel_settings.h"

Plate::Plate(DEBUG debug = DEBUG::NONE) {
  // Constructor
  this->debug = debug;

  current_time = 0;
  previous_time = 0;
  print_time_flag = 0;

  FR_enc_count = 0;
  FL_enc_count = 0;
  BR_enc_count = 0;
  BL_enc_count = 0;

  FR_rpm = 0;
  FL_rpm = 0;
  BR_rpm = 0;
  BL_rpm = 0;
}

Plate::~Plate() {
  // Destructor
}

void Plate::plate_connect() {
  FR.wheel_connect(FR_dig_pin_1, FR_dig_pin_2, FR_pwm_pin, FR_enc_pin_a, FR_enc_pin_b, FR_pos_kp, FR_pos_ki, FR_pos_kd, FR_vel_kp, FR_vel_ki, FR_vel_kd);
  FL.wheel_connect(FL_dig_pin_1, FL_dig_pin_2, FL_pwm_pin, FL_enc_pin_a, FL_enc_pin_b, FL_pos_kp, FL_pos_ki, FL_pos_kd, FL_vel_kp, FL_vel_ki, FL_vel_kd);
  BR.wheel_connect(BR_dig_pin_1, BR_dig_pin_2, BR_pwm_pin, BR_enc_pin_a, BR_enc_pin_b, BR_pos_kp, BR_pos_ki, BR_pos_kd, FL_vel_kp, FL_vel_ki, FL_vel_kd);
  BL.wheel_connect(BL_dig_pin_1, BL_dig_pin_2, BL_pwm_pin, BL_enc_pin_a, BL_enc_pin_b, BL_pos_kp, BL_pos_ki, BL_pos_kd, FL_vel_kp, FL_vel_ki, FL_vel_kd);
  // arm.arm_connect(ARM_pin_0, ARM_pin_1, ARM_pin_2, ARM_pin_3, ARM_pin_4, ARM_pin_gripper);

  if(debug == DEBUG::PLOT) {
    plotter.Begin();
    plotter.AddTimeGraph("Motor position", 5000, "FR", FR_enc_count, "BR", BR_enc_count);
    plotter.AddTimeGraph("Motor rpm", 5000, "FR", FR_rpm, "BR", BR_rpm);
  } else if(debug == DEBUG::TEXT) {
    Serial.begin(9600);
  }
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

bool Plate::plate_command(Command command, double value) {
  // units: meter, radian, m/s, rad/s, pwm
  double target_pwm, target_pos, target_vel;

  plate_update_state();
  switch (command) {

  case Command::LINEAR_PWM:
    target_pwm = value;

    FR.wheel_pwm_ctrl(target_pwm);
    FL.wheel_pwm_ctrl(target_pwm);
    BR.wheel_pwm_ctrl(target_pwm);
    BL.wheel_pwm_ctrl(target_pwm);
    break;

  case Command::LINEAR_POS:
    target_pos = value * PULSE_PER_METER; // pulses
    
    FR.wheel_pos_ctrl(target_pos);
    FL.wheel_pos_ctrl(target_pos);
    BR.wheel_pos_ctrl(target_pos);
    BL.wheel_pos_ctrl(target_pos);
    break;

  case Command::LINEAR_VEL:
    target_vel = value * PULSE_PER_METER; // pulses per second

    FR.wheel_vel_ctrl(target_vel);
    FL.wheel_vel_ctrl(target_vel);
    BR.wheel_vel_ctrl(target_vel);
    BL.wheel_vel_ctrl(target_vel);
    break;

  case Command::LINEAR_COMP:
    target_pos = value * PULSE_PER_METER; // pulses
    target_vel = 0.5 * PULSE_PER_METER; // pulses per second

    FR.wheel_comp_ctrl(target_vel, target_pos);
    FL.wheel_comp_ctrl(target_vel, target_pos);
    BR.wheel_comp_ctrl(target_vel, target_pos);
    BL.wheel_comp_ctrl(target_vel, target_pos);
    break;

  case Command::ANGULAR_PWM:
    target_pwm = value;

    FR.wheel_pwm_ctrl(target_pwm);
    FL.wheel_pwm_ctrl(-target_pwm);
    BR.wheel_pwm_ctrl(target_pwm);
    BL.wheel_pwm_ctrl(-target_pwm);
    break;
    
  case Command::ANGULAR_POS:
    target_pos = value * PULSE_PER_METER; // pulses

    FR.wheel_pos_ctrl(target_pos);
    FL.wheel_pos_ctrl(-target_pos);
    BR.wheel_pos_ctrl(target_pos);
    BL.wheel_pos_ctrl(-target_pos);
    break;

  case Command::ANGULAR_VEL:
    target_vel = value * PULSE_PER_METER; // pulses per second

    FR.wheel_vel_ctrl(target_vel);
    FL.wheel_vel_ctrl(-target_vel);
    BR.wheel_vel_ctrl(target_vel);
    BL.wheel_vel_ctrl(-target_vel);
    break;

  case Command::ANGULAR_COMP:
    target_pos = value * PULSE_PER_METER; // pulses
    target_vel = 0.5 * PULSE_PER_METER; // pulses per second

    FR.wheel_comp_ctrl(target_vel, target_pos);
    FL.wheel_comp_ctrl(-target_vel, target_pos);
    BR.wheel_comp_ctrl(target_vel, target_pos);
    BL.wheel_comp_ctrl(-target_vel, target_pos);
    break;

  case Command::PAUSE:
    FR.wheel_pwm_ctrl(0);
    FL.wheel_pwm_ctrl(0);
    BR.wheel_pwm_ctrl(0);
    BL.wheel_pwm_ctrl(0);
    break;

  case Command::RESET:
    FR.wheel_rest_enc();
    FL.wheel_rest_enc();
    BR.wheel_rest_enc();
    BL.wheel_rest_enc();
    break;

  case Command::DEBUG:
    if(debug == DEBUG::TEXT) {
      plate_print_info();
    } else if(debug == DEBUG::PLOT) {
      plotter.Plot();
    }
    break;

  default:
    break;
  }
  return true;
}

// bool Plate::plate_before_take_ball(){

// }

// bool Plate::plate_take_ball(){

// }

// bool Plate::plate_release_ball(){

// }

void Plate::plate_print_info() {
  // Print every 0.1 second
  if(current_time - print_time_flag < 20000) {
    return;
  }
  print_time_flag = current_time;

  const char* separator = " | ";
  // const char* horizontal_line = "------------------------";

  for(int i = 0; i < 22; i++) {
    Serial.print("\n");
  }
  
  // Serial.println(horizontal_line);
  Serial.print("| FR: ");
  Serial.print((double)FR_enc_count / 100.0, 2);
  Serial.print(separator);
  Serial.print((double)FR_rpm / 100.0, 2);
  Serial.println(" |");

  // Serial.println(horizontal_line);
  Serial.print("| FL: ");
  Serial.print((double)FL_enc_count / 100.0, 2);
  Serial.print(separator);
  Serial.print((double)FL_rpm / 100.0, 2);
  Serial.println(" |");

  // Serial.println(horizontal_line);
  Serial.print("| BR: ");
  Serial.print((double)BR_enc_count / 100.0, 2);
  Serial.print(separator);
  Serial.print((double)BR_rpm / 100.0, 2);
  Serial.println(" |");

  // Serial.println(horizontal_line);
  Serial.print("| BL: ");
  Serial.print((double)BL_enc_count / 100.0, 2);
  Serial.print(separator);
  Serial.print((double)BL_rpm / 100.0, 2);
  Serial.println(" |");

  // Serial.println(horizontal_line);
}

void Plate::plate_rest_enc() {
  noInterrupts();
  FR.wheel_rest_enc();
  FL.wheel_rest_enc();
  BR.wheel_rest_enc();
  BL.wheel_rest_enc();
  interrupts();
}

void Plate::plate_update_state() {
  noInterrupts();
  FR_enc_count = FR.get_encoder_count();
  FL_enc_count = FL.get_encoder_count();
  BR_enc_count = BR.get_encoder_count();
  BL_enc_count = BL.get_encoder_count();
  
  FR_rpm = FR.get_motor_vel() / PULSE_PER_TURN;
  FL_rpm = FL.get_motor_vel() / PULSE_PER_TURN;
  BR_rpm = BR.get_motor_vel() / PULSE_PER_TURN;
  BL_rpm = BL.get_motor_vel() / PULSE_PER_TURN;
  interrupts();
  
  previous_time = current_time;
  current_time = micros();
}