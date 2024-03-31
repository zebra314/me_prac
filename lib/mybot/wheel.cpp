#include "wheel.h"

Wheel::Wheel() {
  // Constructor

  // Encoder
  encoder_count = 0;
  interrupt_num = 0;

  // vels
  vel_time_flag = micros();
  encoder_count_flag = 0;
  vel = 0;

  // PID params
  prev_pos_time = 0;
  prev_pos_error = 0;
  pos_error_integral = 0;

  prev_vel_time = 0;
  prev_vel_error = 0;
  vel_error_integral = 0;
}

Wheel::~Wheel() {
  // Destructor
  detachInterrupt(this->interrupt_num);
}

Wheel* wheel_instance[6] = {nullptr};

template <int interrupt_num>
void encoder_isr() {
  wheel_instance[interrupt_num]->encoder_read();
}

void Wheel::wheel_connect(byte dc_pin_dig_1, byte dc_pin_dig_2, byte dc_pin_pwm, 
                          byte encoder_pin_a, byte encoder_pin_b,
                          double pos_kp, double pos_ki, double pos_kd,
                          double vel_kp, double vel_ki, double vel_kd) {

  this->dc_pin_dig_1 = dc_pin_dig_1;
  this->dc_pin_dig_2 = dc_pin_dig_2;
  this->dc_pin_pwm = dc_pin_pwm;

  this->encoder_pin_a = encoder_pin_a;
  this->encoder_pin_b = encoder_pin_b;
  this->interrupt_num = digitalPinToInterrupt(encoder_pin_a);

  this->pos_kp = pos_kp;
  this->pos_kd = pos_kd;
  this->pos_ki = pos_ki;

  this->vel_kp = vel_kp;
  this->vel_kd = vel_kd;
  this->vel_ki = vel_ki;

  pinMode(this->dc_pin_dig_1, OUTPUT);
  pinMode(this->dc_pin_dig_2, OUTPUT);
  pinMode(this->dc_pin_pwm, OUTPUT);

  pinMode(this->encoder_pin_a, INPUT);
  pinMode(this->encoder_pin_b, INPUT);

  // The encoder_isr function is a template function, whcih takes the a constant integer as a template argument.
  #ifdef ENABLE_ENCODER
  switch (interrupt_num) {
    case 0:
      attachInterrupt(interrupt_num, encoder_isr<0>, RISING);
      break;
    case 1:
      attachInterrupt(interrupt_num, encoder_isr<1>, RISING);
      break;
    case 4:
      attachInterrupt(interrupt_num, encoder_isr<4>, RISING);
      break;
    case 5:
      attachInterrupt(interrupt_num, encoder_isr<5>, RISING);
      break;
    default:
      break;
  }
  #endif
  wheel_instance[interrupt_num] = this;
}

void Wheel::encoder_read() {
  if (digitalRead(this->encoder_pin_b) == LOW) {
    this->encoder_count--;
  } else {
    this->encoder_count++;
  }
  
  double dt = micros() - this->vel_time_flag;
  if (dt > VEL_TRIGGER_LOWER_BOUND) {
    this->vel = (encoder_count - this->encoder_count_flag) * 60.0 / (PPR * GEAR_RATIO) / (dt / 1.0e6);
    this->encoder_count_flag = encoder_count;
    this->vel_time_flag = micros();
  }

}

void Wheel::wheel_pwm_ctrl(double pwm_) {
  int pwm = (int) pwm_;
  if (pwm > 0) {
    digitalWrite(this->dc_pin_dig_1, HIGH);
    digitalWrite(this->dc_pin_dig_2, LOW);
  } else if (pwm < 0) {
    digitalWrite(this->dc_pin_dig_1, LOW);
    digitalWrite(this->dc_pin_dig_2, HIGH);
  } else {
    digitalWrite(this->dc_pin_dig_1, LOW);
    digitalWrite(this->dc_pin_dig_2, LOW);
  }

  if(pwm > 255) {
    pwm = 255;
  } else if(pwm < -255) {
    pwm = -255;
  }
  analogWrite(this->dc_pin_pwm, abs(pwm));
}

void Wheel::wheel_pos_ctrl(double pos) {
  int pwm = (int) (pos_pid(pos));
  wheel_pwm_ctrl(pwm);
}

void Wheel::wheel_vel_ctrl(double vel) {
  int pwm = (int) (vel_pid(vel));
  wheel_pwm_ctrl(pwm);
}

void Wheel::wheel_comp_ctrl(double vel, double pos) {
  int pwm = (int) (comp_pid(vel, pos));
  wheel_pwm_ctrl(pwm);
}

double Wheel::pos_pid(double target) {
  double current_time = micros();
  double dt = (current_time - this->prev_pos_time) / 1.0e6;

  double error = wheel_instance[this->interrupt_num]->encoder_count - target;
  double error_derivative = (error - this->prev_pos_error) / dt;
  this->pos_error_integral += error * dt;

  double output = this->pos_kp * error + this->pos_ki * this->pos_error_integral + this->pos_kd * error_derivative;

  this->prev_pos_time = current_time;
  this->prev_pos_error = error;

  return output;
}

double Wheel::vel_pid(double target) {
  double current_time = micros();
  double dt = (current_time - this->prev_vel_time) / 1.0e6;

  double error = wheel_instance[this->interrupt_num]->vel - target;
  double error_derivative = (error - this->prev_vel_error) / dt;
  this->vel_error_integral += error * dt;

  double output = this->vel_kp * error + this->vel_ki * this->vel_error_integral + this->vel_kd * error_derivative;

  this->prev_vel_time = current_time;
  this->prev_vel_error = error;

  return output;
}

double Wheel::comp_pid(double target_vel_, double target_pos) {
  double target_vel = fabs(target_vel_);
  
  double current_time = micros();
  double dt = (current_time - this->prev_vel_time) / 1.0e6;

  double pos_error = wheel_instance[this->interrupt_num]->encoder_count - target_pos;
  double vel_error = wheel_instance[this->interrupt_num]->vel - target_vel;

  double pos_error_derivative = (pos_error - this->prev_pos_error) / dt;
  double vel_error_derivative = (vel_error - this->prev_vel_error) / dt;
  
  this->pos_error_integral += pos_error * dt;
  this->vel_error_integral += vel_error * dt;

  this->pos_error_max = fmax(abs(this->pos_error_max), abs(pos_error));

  double pos_output = this->pos_kp * pos_error + this->pos_ki * this->pos_error_integral + this->pos_kd * pos_error_derivative;
  double vel_output = this->vel_kp * vel_error + this->vel_ki * this->vel_error_integral + this->vel_kd * vel_error_derivative;

  double pos_weight_base = 100;
  double pos_error_percent = fabs(pos_error) / this->pos_error_max; // Value domain: [0, 1]
  double vel_weight = (pow(pos_weight_base, pos_error_percent) - 1) / (pos_weight_base - 1); // Value domain: [~0, 1]
  double pos_weight= 1 - vel_weight;
  
  double output = pos_output * pos_weight + vel_output * vel_weight;

  this->prev_pos_time = current_time;
  this->prev_vel_time = current_time;

  this->prev_pos_error = pos_error;
  this->prev_vel_error = vel_error;

  return output;
}

double Wheel::get_encoder_count() {
  return wheel_instance[this->interrupt_num]->encoder_count;
}

double Wheel::get_motor_vel() {
  double current_time = micros();
  if ((current_time - this->vel_time_flag) > VEL_TRIGGER_UPPER_BOUND) {
    this->vel = 0;
  }

  return wheel_instance[this->interrupt_num]->vel;
}

void Wheel::wheel_rest_enc() {
  wheel_instance[this->interrupt_num]->encoder_count = 0;
}