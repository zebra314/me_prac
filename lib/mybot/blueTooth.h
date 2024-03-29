#ifndef ARM_H
#define ARM_H

#include <SoftwareSerial.h> 

#define BlueTooth_TX_pin 11
#define BlueTooth_RX_pin 10
#define BlueTooth_BaudRate 9600

void BlueTooth_setup();
bool BlueTooth_read(char*);

#endif