#include "ext_servo.h"

ExtServo::ExtServo() {
  // Constructor
}

ExtServo::~ExtServo() {
  // Destructor
}

// 53 is the biggest number of external servos
ExtServo* ext_servo_list[53] = {nullptr};

template <int attach_pin>
static void ext_servo_handler(void* pvParameters) {
  while (true) {
    Serial.println(attach_pin);
    // if (ext_servo_list[attach_pin]->current_deg == ext_servo_list[attach_pin]->target_deg) {
    //   break;
    // }else if (ext_servo_list[attach_pin]->current_deg < ext_servo_list[attach_pin]->target_deg) {
    //   ext_servo_list[attach_pin]->current_deg++;
    // } else if (ext_servo_list[attach_pin]->current_deg > ext_servo_list[attach_pin]->target_deg) {
    //   ext_servo_list[attach_pin]->current_deg--;
    // }

    // // Slow down the servo movement when it's close to the target
    // if(abs(ext_servo_list[attach_pin]->current_deg - ext_servo_list[attach_pin]->target_deg) < 15) {
    //   ext_servo_list[attach_pin]->ms_delay *= 1.1;
    // }

    // ext_servo_list[attach_pin]->servo.write(ext_servo_list[attach_pin]->current_deg);
    vTaskDelay(ext_servo_list[attach_pin]->ms_delay / portTICK_PERIOD_MS);

    // Serial.print(attach_pin); Serial.print(" "); Serial.println(ext_servo_list[attach_pin]->current_deg);
  }
  // ext_servo_list[attach_pin]->ext_servo_suspend();
}

void ExtServo::ext_servo_connect(int attach_pin, int init_deg, int upper_limit, int lower_limit) {
  this->servo.attach(attach_pin);

  this->attach_pin = attach_pin;
  this->init_deg = init_deg;
  this->upper_limit = upper_limit;
  this->lower_limit = lower_limit;
  
  ext_servo_zero();
  ext_servo_allot(attach_pin);
}

void ExtServo::ext_servo_allot(int attach_pin) {
  switch (attach_pin) {
  case 8:
    xTaskCreate(ext_servo_handler<8>, "ext_servo_handler", 1000, NULL, 1, NULL);
    ext_servo_list[8] = this;
    break;
  
  case 9:
    xTaskCreate(&ext_servo_handler<9>, "ext_servo_handler", 1000, NULL, 1, NULL);
    ext_servo_list[9] = this;
    break;
  
  default:
    break;
  }
}

void ExtServo::ext_servo_zero() {
  this->servo.write(this->init_deg);
  this->current_deg = this->init_deg;
  Serial.println("Zero Done");
}

void ExtServo::ext_servo_set(int deg, int ms_delay) {
  Serial.println("Setting");
  if (deg > this->upper_limit) deg = this->upper_limit;
  if (deg < this->lower_limit) deg = this->lower_limit;

  this->target_deg = deg;
  this->ms_delay = ms_delay;
}

void ExtServo::ext_servo_suspend() {
  Serial.println(F("Suspending"));
  vTaskSuspend(NULL);
}

void ExtServo::ext_servo_resume() {
  Serial.println(F("Resuming"));
  vTaskResume(NULL);
}