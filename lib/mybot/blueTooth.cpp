#include "blueTooth.h"

SoftwareSerial BT(BlueTooth_RX_pin, BlueTooth_TX_pin);

void BlueTooth_setup(){
    BT.begin(BlueTooth_BaudRate);
}

bool BlueTooth_read(char* rcv){
    if (BT.available()) {
        *rcv = BT.read();
        return true;
    }else{
        return false;
    }
}