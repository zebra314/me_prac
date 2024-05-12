#ifndef PLATE_H
#define PLATE_H

#include "wheel.h"
#include "arm.h"
#include "wheel_settings.h"
#include <Plotter.h>

enum class Command:char {
  LINEAR_PWM,
  LINEAR_POS,
  LINEAR_VEL,
  LINEAR_COMP,
  ANGULAR_PWM,
  ANGULAR_POS,
  ANGULAR_VEL,
  ANGULAR_COMP,
  PAUSE,
  RESET,
  DEBUG,
};

enum class DEBUG:char {
  PLOT,
  TEXT,
  NONE,
};

enum class WheelType:char {
  FR,
  FL,
  BR,
  BL,
};

class Plate {
private:
  Wheel FR;
  Wheel FL;
  Wheel BR;
  Wheel BL;
  // Arm arm;
  DEBUG debug;
  Plotter plotter;

  double FR_enc_count;
  double FL_enc_count;
  double BR_enc_count;
  double BL_enc_count;

  double FR_rpm;
  double FL_rpm;
  double BR_rpm;
  double BL_rpm;

  double current_time;
  double previous_time;
  double print_time_flag;

  void plate_print_info();
  void plate_update_state();

public:
  Plate(DEBUG debug = DEBUG::NONE);
  ~Plate();

  int serial_input;
  int serial_buffer;

  /* GAME */
  void plate_before_take_ball();
  // bool plate_take_ball();
  // bool plate_release_ball();
  // void plate_manual();

  /* BASIC */
  void plate_connect();
  bool plate_command(Command command, double value_1 = 0, double value_2 = 0);
  void plate_rest_enc();
  void plate_get_serial_input();

  Arm arm;
};

#endif // !PLATE_H