#include <Arduino_FreeRTOS.h>
#include <arm.h>
#include <plate.h>
#include <blueTooth.cpp>

// Mybot components
Plate plate;
Arm arm;

// BT data
int rcv = 0x04;
int vel = 0;

// Game Logic
bool executed = false;

// Motor timeout control
bool motor_timeout_executed = false;
double last_time = millis();

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

    if(millis() - last_time > 150 && !motor_timeout_executed) {
      plate.plate_command(Command::PAUSE, 0);
      motor_timeout_executed = true;
    }

    // Read the bluetooth signal and translate it to command
    if(BlueTooth_read(&rcv, &vel)) {
      // Serial.print(rcv); Serial.print(" "); Serial.println(vel);
      switch (rcv) {
      case 16:
        plate.plate_command(Command::LINEAR_PWM, 200);
        last_time = millis();
        motor_timeout_executed = false;
        break;

      case 17:
        plate.plate_command(Command::LINEAR_PWM, -200);
        last_time = millis();
        motor_timeout_executed = false;
        break;

      case 18:
        plate.plate_command(Command::ANGULAR_PWM, 200);
        last_time = millis();
        motor_timeout_executed = false;
        break;

      case 19:
        plate.plate_command(Command::ANGULAR_PWM, -200);
        last_time = millis();
        motor_timeout_executed = false;
        break;

      case 20:
        plate.plate_command(Command::PAUSE, 0);
        last_time = millis();
        motor_timeout_executed = false;
        break;

      case 33:
        arm.arm_mv_delta(1, 5, 40);
        break;
      
      case 34:
        arm.arm_mv_delta(1, -5, 40);
        break;
      
      case 35:
        arm.arm_mv_delta(2, 5, 20);
        break;
      
      case 36:
        arm.arm_mv_delta(2, -5, 20);
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
      
      default:
        break;
      }
    }
  }
  vTaskDelete(NULL);
}