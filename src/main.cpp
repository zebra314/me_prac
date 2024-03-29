#include <Arduino.h>
#include "plate.h"
#include "blueTooth.cpp"

// these are bluetooth related code
char rcv = 0x04;
Command cmd;

Plate plate(DEBUG::NONE);

void setup() {
  Serial.begin(9600);
  plate.plate_connect();
  BlueTooth_setup();
  Serial.println("Setup done");
}

void loop() {
  // read the bluetooth signal and translate it to command
  if(BlueTooth_read(&rcv)){
    switch (rcv)
    {
    case '0':
      Serial.println("FORWARD");
      plate.plate_command(Command::LINEAR_PWM, 120);
      break;

    case '1':
      Serial.println("BACKWARD");
      plate.plate_command(Command::LINEAR_PWM, -120);
      break;

    case '2':
      Serial.println("CLOCKWISE");
      plate.plate_command(Command::ANGULAR_PWM, 120);
      break;

    case '3':
      Serial.println("COUNTER_CLOCKWISE");
      plate.plate_command(Command::ANGULAR_PWM, -120);
      break;
    
    case '4':
      Serial.println("STOP");
      plate.plate_command(Command::PAUSE, 0);
      break;

    default:
      Serial.println(rcv);
      break;
    }
  }
  
  // plate.plate_command(Command::DEBUG, 0);
}
