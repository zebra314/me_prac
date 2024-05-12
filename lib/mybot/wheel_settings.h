#ifndef SETTINGS_H
#define SETTINGS_H

/*
  Arduino pins that usable for intrrupts:
  - Uno, Nano, Mini, other 328-based: 2, 3
  - Mega, Mega2560, MegaADK: 2, 3, 18, 19, 20, 21
*/

/* BASIC WHEEL */
#define PPR 11
#define GEAR_RATIO 50
#define WHEEL_DIAMETER 0.1
#define PULSE_PER_TURN (PPR * GEAR_RATIO)
#define PULSE_PER_METER (PULSE_PER_TURN / (WHEEL_DIAMETER * PI))

/* FRONT LEFT WHEEL */
#define FL_pwm_pin   11
#define FL_dig_pin_1 51
#define FL_dig_pin_2 49
#define FL_enc_pin_a 2
#define FL_enc_pin_b 4

#define FL_pos_kp    2.7
#define FL_pos_ki    0.04
#define FL_pos_kd    0.8

#define FL_vel_kp    2.7
#define FL_vel_ki    0.04
#define FL_vel_kd    0.8

/* BACK LEFT WHEEL */
#define BL_pwm_pin   10
#define BL_dig_pin_1 45
#define BL_dig_pin_2 47
#define BL_enc_pin_a 3
#define BL_enc_pin_b 5

#define BL_pos_kp    2.7
#define BL_pos_ki    0.04
#define BL_pos_kd    0.8

#define BL_vel_kp    2.7
#define BL_vel_ki    6
#define BL_vel_kd    0

/* FRONT RIGHT WHEEL */
#define FR_pwm_pin   9
#define FR_dig_pin_1 39
#define FR_dig_pin_2 37
#define FR_enc_pin_a 18
#define FR_enc_pin_b 16

#define FR_pos_kp    2.7
#define FR_pos_ki    0.04
#define FR_pos_kd    0.8

#define FR_vel_kp    2.7
#define FR_vel_ki    6
#define FR_vel_kd    0

/* BACK RIGHT WHEEL */
#define BR_pwm_pin   8
#define BR_dig_pin_1 33
#define BR_dig_pin_2 35
#define BR_enc_pin_a 19
#define BR_enc_pin_b 17

#define BR_pos_kp    2.7
#define BR_pos_ki    0.04
#define BR_pos_kd    0.8

#define BR_vel_kp    2.7
#define BR_vel_ki    0.04
#define BR_vel_kd    0.8

/* ARM */
#define ARM_pin_0 22
#define ARM_pin_1 24
#define ARM_pin_2 26
#define ARM_pin_3 28

#endif // !SETTINGS_H