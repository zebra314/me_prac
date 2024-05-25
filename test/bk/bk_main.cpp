#include <Arduino.h>
#include <plate.h>
#include <arm.h>
#include "blueTooth.cpp"

// BT param
int rcv = 0x04;
int vel = 0;

Plate plate;
Arm arm;

void setup() {
  Serial.begin(9600);
  BlueTooth_setup();
  plate.plate_connect();
  arm.arm_connect();
}

bool executed = false;
double last_time = millis();

void loop() {

  // Game logic
  if (!executed) {
    // plate.plate_before_take_ball();
    // arm.arm_set_pos(ARM_POS::TAKE_BALL);
    // arm.arm_set_pos(ARM_POS::PRE_TAKE_BALL);
    // executed = true;
  }

  if(millis() - last_time > 150 ){
    plate.plate_command(Command::PAUSE, 0);
  }

  // Read the bluetooth signal and translate it to command
  if(BlueTooth_read(&rcv, &vel)){
    // Serial.print(rcv); Serial.print(" "); Serial.println(vel);
    switch (rcv)
    {
    case 16:
      plate.plate_command(Command::LINEAR_PWM, 200);
      last_time = millis();
      break;

    case 17:
      plate.plate_command(Command::LINEAR_PWM, -200);
      last_time = millis();
      break;

    case 18:
      plate.plate_command(Command::ANGULAR_PWM, 200);
      last_time = millis();
      break;

    case 19:
      plate.plate_command(Command::ANGULAR_PWM, -200);
      last_time = millis();
      break;

    case 20:
      plate.plate_command(Command::PAUSE, 0);
      last_time = millis();
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
