#ifndef EXT_SERVO_H
#define EXT_SERVO_H

#include <Arduino.h>
#include <Servo.h>
#include <Arduino_FreeRTOS.h>

class ExtServo {
private:
  int attach_pin;
  int init_deg;
  int upper_limit;
  int lower_limit;

  void ext_servo_allot(int attach_pin);

public:
  ExtServo();
  ~ExtServo();
  Servo servo;
  TaskHandle_t* taskHandle;

  int target_deg;
  int current_deg;
  int ms_delay;

  void ext_servo_connect(int attach_pin, int init_deg, int upper_limit, int lower_limit);
  void ext_servo_zero();
  void ext_servo_set(int deg, int ms_delay);

  // Xtask handler
  void ext_servo_suspend();
  void ext_servo_resume();

};

#endif // !EXT_SERVO_H