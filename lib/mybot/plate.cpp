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

  FR_rpms = 0;
  FL_rpms = 0;
  BR_rpms = 0;
  BL_rpms = 0;

  FR_target = 0;
  FL_target = 0;
  BR_target = 0;
  BL_target = 0;

}

Plate::~Plate() {
  // Destructor
}

void Plate::plate_connect() {
  // FR.wheel_connect(FR_dig_pin_1, FR_dig_pin_2, FR_pwm_pin, FR_enc_pin_a, FR_enc_pin_b, FR_kp, FR_ki, FR_kd);
  FL.wheel_connect(FL_dig_pin_1, FL_dig_pin_2, FL_pwm_pin, FL_enc_pin_a, FL_enc_pin_b, FL_kp, FL_ki, FL_kd);
  // BR.wheel_connect(BR_dig_pin_1, BR_dig_pin_2, BR_pwm_pin, BR_enc_pin_a, BR_enc_pin_b, BR_kp, BR_ki, BR_kd);
  // BL.wheel_connect(BL_dig_pin_1, BL_dig_pin_2, BL_pwm_pin, BL_enc_pin_a, BL_enc_pin_b, BL_kp, BL_ki, BL_kd);
  // arm.arm_connect(ARM_pin_0, ARM_pin_1, ARM_pin_2, ARM_pin_3, ARM_pin_4, ARM_pin_gripper);

  if(debug == DEBUG::PLOT) {
    plotter.Begin();
    plotter.AddTimeGraph("Motor position", 5000, "FL", FL_enc_count);
    plotter.AddTimeGraph("Motor RPMs", 5000, "FL", FL_rpms);
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

bool done = false;
bool not_done = true;
bool Plate::plate_command(Command command, float value) {
  plate_update_state();

  if(command == Command::DEBUG && debug == DEBUG::TEXT) {
    plate_check_info();
    return done;
  } else if(command == Command::DEBUG && debug == DEBUG::PLOT) {
    plotter.Plot();
    return done;
  } else if(command == Command::PAUSE || command == Command::RECORD) {
      FR.wheel_pwm_ctrl(0);
      FL.wheel_pwm_ctrl(0);
      BR.wheel_pwm_ctrl(0);
      BL.wheel_pwm_ctrl(0);
    return done;
  }

  float velocity, omega;
  float delta_target;
  float pulse_per_turn = PPR * GEAR_RATIO;
  float pulse_per_meter = pulse_per_turn / (WHEEL_DIAMETER * PI);

  switch (command) {
    case Command::LINEAR_POSI:
      velocity = 0.4;
      delta_target = velocity * pulse_per_meter * (current_time - previous_time) / 1.0e6;

      FR_target += delta_target;
      FL_target += delta_target;
      BR_target += delta_target;
      BL_target += delta_target;
      Plate::plate_move();
      break;
    
    case Command::ANGULAR_POSI:
      FR.wheel_posi_ctrl(value);
      FL.wheel_posi_ctrl(-value);
      BR.wheel_posi_ctrl(value);
      BL.wheel_posi_ctrl(-value);
      break;
    
    case Command::LINEAR_VEL:
      velocity = value;
      delta_target = velocity * pulse_per_meter * (current_time - previous_time) / 1.0e6;

      FR_target += delta_target;
      FL_target += delta_target;
      BR_target += delta_target;
      BL_target += delta_target;

      Plate::plate_move();
      break;

    case Command::ANGULAR_VEL:
      omega = value; 
      delta_target = omega * pulse_per_meter * (current_time - previous_time) / 1.0e6;

      FR_target += delta_target;
      FL_target -= delta_target;
      BR_target += delta_target;
      BL_target -= delta_target;

      Plate::plate_move();
      break;

    // case Command::PAUSE:
    //   FR.wheel_pwm_ctrl(0);
    //   FL.wheel_pwm_ctrl(0);
    //   BR.wheel_pwm_ctrl(0);
    //   BL.wheel_pwm_ctrl(0);
    //   break;

    // case Command::RECORD:
    //   FR.wheel_pwm_ctrl(0);
    //   FL.wheel_pwm_ctrl(0);
    //   BR.wheel_pwm_ctrl(0);
    //   BL.wheel_pwm_ctrl(0);
    //   break;
  }
  return done;
}

// bool Plate::plate_before_take_ball(){

// }

// bool Plate::plate_take_ball(){

// }

// bool Plate::plate_release_ball(){

// }

void Plate::plate_check_info() {
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
  Serial.print((float)FR_enc_count / 100.0, 2);
  Serial.print(separator);
  Serial.print((float)FR_rpms / 100.0, 2);
  Serial.println(" |");

  // Serial.println(horizontal_line);
  Serial.print("| FL: ");
  Serial.print((float)FL_enc_count / 100.0, 2);
  Serial.print(separator);
  Serial.print((float)FL_rpms / 100.0, 2);
  Serial.println(" |");

  // Serial.println(horizontal_line);
  Serial.print("| BR: ");
  Serial.print((float)BR_enc_count / 100.0, 2);
  Serial.print(separator);
  Serial.print((float)BR_rpms / 100.0, 2);
  Serial.println(" |");

  // Serial.println(horizontal_line);
  Serial.print("| BL: ");
  Serial.print((float)BL_enc_count / 100.0, 2);
  Serial.print(separator);
  Serial.print((float)BL_rpms / 100.0, 2);
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
  
  FR_rpms = FR.get_motor_rpms();
  FL_rpms = FL.get_motor_rpms();
  BR_rpms = BR.get_motor_rpms();
  BL_rpms = BL.get_motor_rpms();
  interrupts();
  
  previous_time = current_time;
  current_time = micros();

}

void Plate::plate_move() {
  FR.wheel_posi_ctrl(FR_target);
  FL.wheel_posi_ctrl(FL_target);
  BR.wheel_posi_ctrl(BR_target);
  BL.wheel_posi_ctrl(BL_target);
}