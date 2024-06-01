#include "ext_servo.h"

ExtServo::ExtServo() {
  // Constructor
}

ExtServo::~ExtServo() {
  // Destructor
}

ExtServo* ext_servo_list[53] = {nullptr}; // 53 is the biggest pin number of external servos
template <int attach_pin>
static void ext_servo_handler(void* pvParameters) {
  while (true) {
    vTaskDelay(1);

    // Check the if archieved the target deg
    if (ext_servo_list[attach_pin]->current_deg == ext_servo_list[attach_pin]->target_deg) {
      // vTaskSuspend(NULL); // This will casusing the servo to shake when the task is suspended
      continue; 
    }
    
    // Check the target deg limit
    if (ext_servo_list[attach_pin]->target_deg > ext_servo_list[attach_pin]->upper_limit) {
      ext_servo_list[attach_pin]->target_deg = ext_servo_list[attach_pin]->upper_limit;
    } else if (ext_servo_list[attach_pin]->current_deg < ext_servo_list[attach_pin]->lower_limit) {
      ext_servo_list[attach_pin]->target_deg = ext_servo_list[attach_pin]->lower_limit;
    }

    // Accelerate at the beginning and slow down when approaching the target
    if (abs(ext_servo_list[attach_pin]->current_deg - ext_servo_list[attach_pin]->target_deg) < 4) {
      ext_servo_list[attach_pin]->mv_unit = 1;
    }

    // Determine the direction of the servo
    if (ext_servo_list[attach_pin]->current_deg < ext_servo_list[attach_pin]->target_deg) {
      ext_servo_list[attach_pin]->current_deg += ext_servo_list[attach_pin]->mv_unit;
    } else if (ext_servo_list[attach_pin]->current_deg > ext_servo_list[attach_pin]->target_deg) {
      ext_servo_list[attach_pin]->current_deg -= ext_servo_list[attach_pin]->mv_unit;
    }

    // Actuate the servo
    ext_servo_list[attach_pin]->servo.write(ext_servo_list[attach_pin]->current_deg);
    vTaskDelay(ext_servo_list[attach_pin]->ms_delay / portTICK_PERIOD_MS);

    // Debug msg
    // ext_servo_list[attach_pin]->ext_servo_show_pos();
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
  this->mv_unit = 1;
  
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