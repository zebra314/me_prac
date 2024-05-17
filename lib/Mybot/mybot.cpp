#include <Arduino_FreeRTOS.h>

#include <arm.h>
#include <plate.h>
#include <blueTooth.cpp>

Plate plate;
Arm arm;

// BT param
int rcv = 0x04;
int vel = 0;

bool executed = false;
void mybot_task(void* pvParameters);

void mybot_connect(){
  BlueTooth_setup();
  plate.plate_connect();
  arm.arm_connect();
  arm.arm_zero();
}

void mybot_on() {
  xTaskCreate(
    mybot_task,
    "mybot_task",
    500,
    NULL,
    0,
    NULL
  );
}

void mybot_task(void* pvParameters) {
  while(1) {
    // Game logic
    if (!executed) {
      // plate.plate_before_take_ball();
      // arm.arm_set_pos(ARM_POS::TAKE_BALL);
      // arm.arm_set_pos(ARM_POS::PRE_TAKE_BALL);
      // executed = true;
    }

    // Read the bluetooth signal and translate it to command
    if(BlueTooth_read(&rcv, &vel)){
      // Serial.print(rcv); Serial.print(" "); Serial.println(vel);
      switch (rcv)
      {
      case 16:
        plate.plate_command(Command::LINEAR_PWM, 200);
        break;

      case 17:
        plate.plate_command(Command::LINEAR_PWM, -200);
        break;

      case 18:
        plate.plate_command(Command::ANGULAR_PWM, 200);
        break;

      case 19:
        plate.plate_command(Command::ANGULAR_PWM, -200);
        break;

      case 20:
        plate.plate_command(Command::PAUSE, 0);
        break;

      case 33:
        break;
      
      case 34:
        break;
      
      case 35:
        break;
      
      case 36:
        break;

      case 37:
        break;
      
      case 38:
        break;
      
      case 39:
        break;
      
      case 40:
        break;
      
      default:
        break;
      }
    }
  }
  vTaskDelete(NULL);
}