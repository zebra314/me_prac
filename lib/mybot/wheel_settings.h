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
#define FL_dig_pin_1 38
#define FL_dig_pin_2 36
#define FL_pwm_pin   20
#define FL_enc_pin_a 18
#define FL_enc_pin_b 15

#define FL_pos_kp    2.7
#define FL_pos_ki    0.04
#define FL_pos_kd    0.8

#define FL_vel_kp    2.7
#define FL_vel_ki    0.04
#define FL_vel_kd    0.8

/* FRONT RIGHT WHEEL */
#define FR_dig_pin_1 4
#define FR_dig_pin_2 5
#define FR_pwm_pin   6
#define FR_enc_pin_a 2
#define FR_enc_pin_b 7

#define FR_pos_kp    2.7
#define FR_pos_ki    6
#define FR_pos_kd    0

#define FR_vel_kp    2.7
#define FR_vel_ki    6
#define FR_vel_kd    0

/* BACK LEFT WHEEL */
#define BL_dig_pin_1 12
#define BL_dig_pin_2 13
#define BL_pwm_pin   14
#define BL_enc_pin_a 19
#define BL_enc_pin_b 21

#define BL_pos_kp    2.7
#define BL_pos_ki    0.04
#define BL_pos_kd    0.8

#define BL_vel_kp    2.7
#define BL_vel_ki    0.04
#define BL_vel_kd    0.8

/* BACK RIGHT WHEEL */
#define BR_dig_pin_1 9
#define BR_dig_pin_2 8
#define BR_pwm_pin   10
#define BR_enc_pin_a 3
#define BR_enc_pin_b 11

#define BR_pos_kp    2.7
#define BR_pos_ki    0.04
#define BR_pos_kd    0.8

#define BR_vel_kp    2.7
#define BR_vel_ki    0.04
#define BR_vel_kd    0.8

/* ARM */
#define ARM_pin_0    22
#define ARM_pin_1    23
#define ARM_pin_2    24
#define ARM_pin_3    25
#define ARM_pin_4    26
#define ARM_pin_gripper 27

#endif // !SETTINGS_H