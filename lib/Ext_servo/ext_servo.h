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

  void ext_servo_allot(int attach_pin);

public:
  ExtServo();
  ~ExtServo();

  // Used in the static servo handler
  Servo servo;

  int upper_limit;
  int lower_limit;
  int target_deg;
  int current_deg;

  // Servo control variables
  int ms_delay;
  int mv_unit;

  void ext_servo_connect(int attach_pin, int init_deg, int lower_limit, int upper_limit);
  void ext_servo_zero();

  // Debug msg
  void ext_servo_show_task_state();
  void ext_servo_show_pos();
};

#endif // !EXT_SERVO_H