#include "ext_servo.h"

ExtServo::ExtServo() {
  // Constructor
}

ExtServo::~ExtServo() {
  // Destructor
}

ExtServo* ext_servo_list[53] = {nullptr}; // 53 is the biggest number of external servos
template <int attach_pin>
static void ext_servo_handler(void* pvParameters) {
  while (true) {
    if (ext_servo_list[attach_pin]->current_deg == ext_servo_list[attach_pin]->target_deg) {
      // vTaskSuspend(NULL); // This will casusing the servo to shake when the task is suspended
      continue; 
    } else if (ext_servo_list[attach_pin]->current_deg < ext_servo_list[attach_pin]->target_deg) {
      ext_servo_list[attach_pin]->current_deg++;
    } else if (ext_servo_list[attach_pin]->current_deg > ext_servo_list[attach_pin]->target_deg) {
      ext_servo_list[attach_pin]->current_deg--;
    }

    ext_servo_list[attach_pin]->servo.write(ext_servo_list[attach_pin]->current_deg);
    vTaskDelay(ext_servo_list[attach_pin]->ms_delay / portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}

void ExtServo::ext_servo_connect(int attach_pin, int init_deg, int lower_limit, int upper_limit) {
  this->servo.attach(attach_pin);
  this->attach_pin = attach_pin;
  this->init_deg = init_deg;
  this->lower_limit = lower_limit;
  this->upper_limit = upper_limit;
  this->ms_delay = 0;
  
  ext_servo_zero();
  ext_servo_allot(attach_pin);
}

void ExtServo::ext_servo_allot(int attach_pin) {
  ext_servo_list[attach_pin] = this;
  ext_servo_list[attach_pin]->taskHandle = xTaskGetCurrentTaskHandle();
  switch (attach_pin) {
  case 22:
    xTaskCreate(ext_servo_handler<22>, "ext_servo_handler", 500, NULL, 1, &taskHandle);
    break;
  
  case 24:
    xTaskCreate(&ext_servo_handler<24>, "ext_servo_handler", 500, NULL, 1, &taskHandle);
    break;
  
  default:
    break;
  }
}

void ExtServo::ext_servo_zero() {
  this->servo.write(this->init_deg);
  this->current_deg = this->init_deg;
  this->target_deg = this->init_deg;
}

void ExtServo::ext_servo_set(int deg, int ms_delay) {
  if (deg > this->upper_limit) deg = this->upper_limit;
  if (deg < this->lower_limit) deg = this->lower_limit;

  this->target_deg = deg;
  this->ms_delay = ms_delay;

  vTaskResume(this->taskHandle);
}

void ExtServo::ext_servo_show_task_state() {
  #if INCLUDE_eTaskGetState
    eTaskState state = eTaskGetState(taskHandle);
    Serial.print("Task state: ");
    Serial.println(state);
  #endif
}

void ExtServo::ext_servo_show_pos() {
  Serial.print(attach_pin);
  Serial.print(" Current deg: ");
  Serial.print(this->current_deg);
  Serial.print(" Target deg: ");
  Serial.println(this->target_deg);
}