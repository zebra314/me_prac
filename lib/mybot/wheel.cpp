#include "wheel.h"

Wheel::Wheel() {
  // Constructor
}

Wheel::~Wheel() {
  // Destructor
  detachInterrupt(digitalPinToInterrupt(this->encoder_pin_a));
}

Wheel* wheel_instance = nullptr;

void encoder_read() {
  if (digitalRead(wheel_instance->encoder_pin_b) == LOW) {
    wheel_instance->encoder_count--;
  } else {
    wheel_instance->encoder_count++;
  }
}

void Wheel::wheel_connect(byte dc_pin_dig_1, byte dc_pin_dig_2, byte dc_pin_pwm, byte encoder_pin_a, byte encoder_pin_b) {
  this->dc_pin_dig_1 = dc_pin_dig_1;
  this->dc_pin_dig_2 = dc_pin_dig_2;
  this->dc_pin_pwm = dc_pin_pwm;

  this->encoder_pin_a = encoder_pin_a;
  this->encoder_pin_b = encoder_pin_b;

  this->kp = 2.5;
  this->kd = 0.8;
  this->ki = 0;

  pinMode(this->dc_pin_dig_1, OUTPUT);
  pinMode(this->dc_pin_dig_2, OUTPUT);
  pinMode(this->dc_pin_pwm, OUTPUT);

  pinMode(this->encoder_pin_a, INPUT);
  pinMode(this->encoder_pin_b, INPUT);

  #ifdef ENABLE_ENCODER
  attachInterrupt(digitalPinToInterrupt(encoder_pin_a), encoder_read, RISING);
  #endif
  
  wheel_instance = this;
}

volatile long Wheel::get_encoder_count() {
  return wheel_instance->encoder_count;
}

void Wheel::wheel_move(int pwm) {
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
  analogWrite(this->dc_pin_pwm, abs(pwm));
}

float Wheel::pid_control(int target) {
  long current_time = micros();
  float dt = (current_time - this->prev_time) / 1.0e6;

  int error = wheel_instance->encoder_count - target;
  float error_derivative = (error - this->prev_error) / dt;
  this->error_integral += error * dt;

  float output = this->kp * error + this->ki * this->error_integral + this->kd * error_derivative;

  this->prev_time = current_time;
  this->prev_error = error;

  return output;
}