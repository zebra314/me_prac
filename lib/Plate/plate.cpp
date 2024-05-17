#include "plate.h"

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

  if(debug == DEBUG::PLOT && !(Serial.available())) {
    plotter.Begin();
    plotter.AddTimeGraph("Motor position", 5000, "FR", FR_enc_count, "BR", BR_enc_count);
    plotter.AddTimeGraph("Motor rpm", 5000, "FR", FR_rpm, "BR", BR_rpm);
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

void Plate::plate_before_take_ball() {
  plate_command(Command::LINEAR_POS, -0.8);
  delay(80);

  // Flash Corner
  plate_command(Command::ANGULAR_POS, -0.17);
  delay(80);

  plate_command(Command::LINEAR_POS, -0.75);
  delay(80);

  plate_command(Command::ANGULAR_POS, 0.23);
  delay(80);

  plate_command(Command::LINEAR_POS, -0.75);
  delay(80);

  plate_command(Command::ANGULAR_POS, -0.145);
  delay(80);

  // 90 deg Corner
  plate_command(Command::LINEAR_POS, -0.52);
  delay(80);

  plate_command(Command::ANGULAR_POS, 0.33);
  delay(80);

  plate_command(Command::LINEAR_POS, -0.4);
  delay(80);
}

/* Encoder pulse output per revolution 
  = Basic pulse x gear ratio

  ChiHai Motor CHR-GM37 1:50, with 11PPR encoder
  = 11 x 50 = 550

  pulse per revolution = 550
  pulse per meter = 550 / (wheel diameter * PI)
*/

bool Plate::plate_command(Command command, double value_1 = 0, double value_2 = 0) {
  // units: meter, radian, m/s, rad/s, pwm
  double target_pwm, target_pos, target_vel;
  double brake_pulses;
  double pos_percent = 0;

  plate_update_state();
  switch (command) {

  case Command::LINEAR_PWM:
    target_pwm = value_1;

    FR.wheel_pwm_ctrl(target_pwm);
    FL.wheel_pwm_ctrl(target_pwm);
    BR.wheel_pwm_ctrl(target_pwm);
    BL.wheel_pwm_ctrl(target_pwm);
    break;

  case Command::LINEAR_POS:
    target_pos = value_1 * PULSE_PER_METER; // pulses
    brake_pulses = 30;
    plate_command(Command::RESET);

    while(true) {
      plate_update_state();
      pos_percent = (FR_enc_count + FL_enc_count + BR_enc_count + BL_enc_count) / 4.0 / target_pos; 
     
      FR.wheel_pos_ctrl(target_pos);
      FL.wheel_pos_ctrl(target_pos);
      BR.wheel_pos_ctrl(target_pos);
      BL.wheel_pos_ctrl(target_pos);

      // Brake if error is small
      if(pos_percent > 0.98) {
        brake_pulses--;
      }
      
      // Exit if arrive target and brake_pulses is 0
      if (brake_pulses <= 0) {
        break;
      }
    }

    plate_command(Command::PAUSE);
    plate_command(Command::RESET);

    break;

  case Command::LINEAR_VEL:
    target_vel = value_1 * PULSE_PER_METER; // pulses per second

    FR.wheel_vel_ctrl(target_vel);
    FL.wheel_vel_ctrl(target_vel);
    BR.wheel_vel_ctrl(target_vel);
    BL.wheel_vel_ctrl(target_vel);
    break;

  case Command::LINEAR_COMP:
    target_pos = value_1 * PULSE_PER_METER; // pulses
    target_vel = value_2 * PULSE_PER_METER; // pulses per second
    
    // while(true) {
    //   FR.wheel_comp_ctrl(target_vel, target_pos);
    //   FL.wheel_comp_ctrl(target_vel, target_pos);
    //   BR.wheel_comp_ctrl(target_vel, target_pos);
    //   BL.wheel_comp_ctrl(target_vel, target_pos);

    //   // Brake if error is small
    //   if(plate_get_pos_error_percent() < 0.05) {
    //     brake_pulses--;
    //   }

    //   // Exit if arrive target and brake_pulses is 0
    //   if(brake_pulses == 0 && plate_get_pos_error_percent() < 0.05) {
    //     break;
    //   }
    // }

    plate_command(Command::PAUSE);
    plate_command(Command::RESET);

    break;

  case Command::ANGULAR_PWM:
    target_pwm = value_1;

    FR.wheel_pwm_ctrl(target_pwm * 0.65);
    FL.wheel_pwm_ctrl(-target_pwm * 0.65);
    BR.wheel_pwm_ctrl(target_pwm);
    BL.wheel_pwm_ctrl(-target_pwm);
    break;
    
  case Command::ANGULAR_POS:
    target_pos = value_1 * PULSE_PER_METER; // pulses
    brake_pulses = 30;

    plate_command(Command::RESET);

    while(true) {
      plate_update_state();
      pos_percent = abs(BR_enc_count - BL_enc_count) / abs(target_pos); 
      
      FR.wheel_pos_ctrl(target_pos);
      FL.wheel_pos_ctrl(-target_pos);
      BR.wheel_pos_ctrl(target_pos);
      BL.wheel_pos_ctrl(-target_pos);

      // Brake if error is small
      if(pos_percent > 0.90) {
        brake_pulses--;
      }

      // Exit if arrive target and brake_pulses is 0
      if (brake_pulses <= 0) {
        break;
      }
    }

    plate_command(Command::PAUSE);
    plate_command(Command::RESET);

    break;

  case Command::ANGULAR_VEL:
    target_vel = value_1 * PULSE_PER_METER; // pulses per second

    FR.wheel_vel_ctrl(target_vel);
    FL.wheel_vel_ctrl(-target_vel);
    BR.wheel_vel_ctrl(target_vel);
    BL.wheel_vel_ctrl(-target_vel);
    break;

  case Command::ANGULAR_COMP:
    target_pos = value_1 * PULSE_PER_METER; // pulses
    target_vel = value_2 * PULSE_PER_METER; // pulses per second

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
  if (!Serial.available()) {
    return;
  }

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
