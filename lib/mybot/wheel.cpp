#include "wheel.h"

Wheel::Wheel() {
  // Constructor

  // Encoder
  encoder_count = 0;
  interrupt_num = 0;

  // RPMs
  rpm_time_flag = micros();
  encoder_count_flag = 0;
  rpms = 0;

  // PID params
  prev_time_pid = 0;
  prev_error = 0;
  error_integral = 0;
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
  Serial.println(this->interrupt_num);
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
  long dt = current_time - this->rpm_time_flag;
  if (dt > 100000) {
    this->encoder_count_flag = encoder_count;
    this->rpm_time_flag = current_time;
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

void Wheel::wheel_posi_ctrl(long posi) {
  int pwm = (int) pid_control(posi);
  wheel_pwm_ctrl(pwm);
}

double Wheel::pid_control(long target) {
  long current_time = micros();
  double dt = (current_time - this->prev_time_pid) / 1.0e6;

  long error = wheel_instance[this->interrupt_num]->encoder_count - target;
  double error_derivative = (error - this->prev_error) / dt;
  this->error_integral += error * dt;

  double output = this->kp * error + this->ki * this->error_integral + this->kd * error_derivative;

  this->prev_time_pid = current_time;
  this->prev_error = error;

  return output;
}

long Wheel::get_encoder_count() {
  return wheel_instance[this->interrupt_num]->encoder_count;
}

long Wheel::get_motor_rpms() {
  long current_time = micros();
  if ((current_time - this->rpm_time_flag) > 700000) {
    this->rpms = 0;
  } else {
    this->rpms = (encoder_count - this->encoder_count_flag) * 60.0 / (PPR * GEAR_RATIO) / ((current_time - this->rpm_time_flag) / 1.0e6);
  }
  return wheel_instance[this->interrupt_num]->rpms;
}

void Wheel::wheel_rest_enc() {
  wheel_instance[this->interrupt_num]->encoder_count = 0;
}