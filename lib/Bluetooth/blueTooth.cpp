#include <SoftwareSerial.h> 

#define BlueTooth_TX_pin 12
#define BlueTooth_RX_pin 13
#define BlueTooth_BaudRate 9600

enum BT_Command{
  FORWARD,
  BACKWARD,
  CLOCKWISE,
  COUNTER_CLOCKWISE,
  STOP
};

SoftwareSerial BT(BlueTooth_RX_pin, BlueTooth_TX_pin);

void BlueTooth_setup() {
  BT.begin(BlueTooth_BaudRate);
}

bool BlueTooth_read(int* rcv, int* vel){
  if (BT.available()) {
    *rcv = BT.read();
    *vel = BT.read();
    return true;
  }else{
    return false;
  }
}