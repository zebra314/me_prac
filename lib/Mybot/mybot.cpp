#include <Arduino_FreeRTOS.h>
#include <arm.h>
#include <plate.h>
#include <blueTooth.cpp>

// Conrtol mode 
#define HIGH_VEL 200
#define LOW_VEL 100

// Mybot components
Plate plate;
Arm arm;

// BT data
int rcv_1 = 0x04;
int rcv_2 = 0x00;
int vel = HIGH_VEL;

// Game Logic
bool executed = false;

// Motor timeout control
bool motor_timeout_executed = false;
double motor_last_time = millis();

// Arm timeout control
bool arm_timeout_executed = false;
double arm_last_time = millis();

static void mybot_task(void* pvParameters);

void mybot_connect()  {
  Serial.begin(9600);
  BlueTooth_setup();
  plate.plate_connect();
  arm.arm_connect();
}

void mybot_on() {
  xTaskCreate(
    mybot_task,
    "mybot_task",
    1000,
    NULL,
    1,
    NULL
  );
}

static void mybot_task(void* pvParameters) {
  while(1) {
    // Game logic
    if (!executed) {
      // plate.plate_before_take_ball();
      // arm.arm_set_pos(ARM_POS::TAKE_BALL);
      // arm.arm_set_pos(ARM_POS::PRE_TAKE_BALL);
      executed = true;
    }

    // Motor timeout control  
    if(millis() - motor_last_time > 75 && !motor_timeout_executed) {
      plate.plate_command(Command::PAUSE, 0);
      motor_timeout_executed = true;
    }

    // Arm timeout control
    if(millis() - arm_last_time > 75 && !arm_timeout_executed) {
      Serial.println(millis() - arm_last_time);
      arm.arm_set_pos(ARM_POS::FREEZE);
      arm_timeout_executed = true;
    }

    // Read the bluetooth signal and translate it to command
    if(BlueTooth_read(&rcv_1, &rcv_2)) {
      // Serial.print(rcv_1); Serial.print(" "); Serial.println(rcv_2);
      switch (rcv_1) {
      case 16:
        plate.plate_command(Command::LINEAR_PWM, vel);
        motor_last_time = millis();
        motor_timeout_executed = false;
        break;

      case 17:
        plate.plate_command(Command::LINEAR_PWM, -vel);
        motor_last_time = millis();
        motor_timeout_executed = false;
        break;

      case 18:
        plate.plate_command(Command::ANGULAR_PWM, vel*1.3);
        motor_last_time = millis();
        motor_timeout_executed = false;
        break;

      case 19:
        plate.plate_command(Command::ANGULAR_PWM, -vel*1.3);
        motor_last_time = millis();
        motor_timeout_executed = false;
        break;

      case 20:
        plate.plate_command(Command::PAUSE, 0);
        motor_last_time = millis();
        motor_timeout_executed = false;
        break;

      case 33:
        arm.arm_mv_delta(1, 5, 17, 2);
        arm_last_time = millis();
        arm_timeout_executed = false;
        break;
      
      case 34:
        arm.arm_mv_delta(1, -5, 17, 2);
        arm_last_time = millis();
        arm_timeout_executed = false;
        break;
      
      case 35:
        arm.arm_mv_delta(2, 5, 17, 2);
        arm_last_time = millis();
        arm_timeout_executed = false;
        break;
      
      case 36:
        arm.arm_mv_delta(2, -5, 17, 2);
        arm_last_time = millis();
        arm_timeout_executed = false;
        break;

      case 37:
        arm.arm_set_pos(ARM_POS::ZERO);
        break;
      
      case 38:
        arm.arm_set_pos(ARM_POS::PRE_TAKE_BALL);
        break;
      
      case 39:
        arm.arm_set_pos(ARM_POS::TAKE_BALL);
        break;
      
      case 40:
        arm.arm_set_pos(ARM_POS::DROP_BALL);
        break;
      
      // Increase the speed
      case 42:
        vel = HIGH_VEL;
        break;

      // Decrease the speed 
      case 43:
        vel = LOW_VEL;
        break;

      default:
        break;
      }
    }
  }
  vTaskDelete(NULL);
}