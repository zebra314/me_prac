#ifndef EXT_SERVO_H
#define EXT_SERVO_H

#include <Arduino.h>
#include <Servo.h>
#include <Arduino_FreeRTOS.h>
#include <FreeRTOSConfig.h>

class ExtServo {
private:
  TaskHandle_t taskHandle;

  int attach_pin;
  int init_deg;
  int upper_limit;
  int lower_limit;

  void ext_servo_allot(int attach_pin);

public:
  ExtServo();
  ~ExtServo();

  Servo servo;

  int target_deg;
  int current_deg;
  int ms_delay;

  void ext_servo_connect(int attach_pin, int init_deg, int lower_limit, int upper_limit);
  void ext_servo_zero();
  void ext_servo_set(int deg, int ms_delay);

  // Debug msg
  void ext_servo_show_task_state();
  void ext_servo_show_pos();
};

#endif // !EXT_SERVO_H