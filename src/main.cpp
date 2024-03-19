#include <Arduino.h>
#include <Wire.h>
#include "plate.h"

/* PINs */
const byte ENCA = 4;
const byte ENCB = 3;
const byte IN3 = 7;
const byte IN4 = 6;
const byte SPD = 5;

Wheel wheel;

int posi = 0;
int TRA = 50;
float duration = 0;
long prevT = 0;
float eprev = 0;
float eintegral = 0;
long recive = 0;
long accumulate = 0;
int phase = 0;
int aim = 0;

int script[20][2]={
  {70,5},
  {140,5},
  {210,5},
  {280,5},
  {350,5},
  {420,5},
  {490,5},
  {560,5},
  {0,20},
  {140,5},
  {280,5},
  {420,5},
  {560,5},
  {0,20},
  {280,10},
  {850,10},
  {1410,10},
  {850,10},
  {280,10},
  {0,20}
};
long last_time = 0;
bool ready = true;

int inputValue = 0;
int buffer;


/* FUNCTIONS */
void readEncoder();
void Motor(int velocity);
void get_serial_input();

void setup() {
  Serial.begin(9600);
  wheel.wheel_connect(IN3, IN4, SPD, ENCA, ENCB);
}


void loop() {
  // int Vel = 0;
  
  
  // float Kp = 10;
  // float Kd = 0.5;
  // float Ki = 0.0;
  
  // if(false){
  //   while(Serial.available() > 0){
  //     int a = Serial.read();
      
  //     if(a==10) {
  //       TRA = recive;
  //       if(TRA == 666) {
  //         ready = false;
  //         TRA = 0;
  //       }else {
  //         aim = recive;
  //       }
  //       recive = 0;
  //     }else{        
  //       recive = recive*10 + (a-48);       
  //     }
      
  //   }
  // }else{
  //   accumulate = micros() - last_time;
  //   if(accumulate > 100000*script[phase][1]){
  //     last_time = micros();
  //     if(phase == 19){
  //       phase = 0;
  //     }else{
  //       phase++;
  //       Serial.println(phase);
  //     }
  //   }
  
  //   TRA =script[phase][0];
  // }
  
  // //Serial.println(TRA);

  // long currT = micros();
  
  // float deltaT = ((float)(currT - prevT))/1.0e6;
  // prevT = currT;
  // if(duration>3){
  //   duration  = 0;
  //   //TRA = TRA + 540*2;
  // }else{
  //   duration += deltaT;
  // }
  // //Serial.println(duration);
  // //Read the position
  // int pos = 0;
  // noInterrupts();
  // pos = posi;
  // interrupts();
  // //error
  // int e = TRA - pos;
  // if(e>1500){
  //   e=1500;
  // }else if(e<-1500){
  //   e=-1500;
  // }
  // //derivative
  // float dedt = (e-eprev)/(deltaT);

  // //integral
  // eintegral = eintegral + e*deltaT;
  // float u = Kp*e + Kd*dedt + Ki*eintegral;
  // Vel = (int)u;
  // Motor(Vel);

  // eprev = e;
  // if(true){
  //   Serial.print(TRA);
  //   Serial.print(" ");
  //   Serial.print(pos);
  //   Serial.print(" ");
  //   Serial.print(u);
  //   Serial.print(" ");
  //   Serial.println(aim);
  // }

  get_serial_input();
  wheel.wheel_move(inputValue);

  }

void readEncoder(){
  int b = digitalRead(ENCB);
  if(b>0){
    posi--;
  }else{
    posi++;
  }

}
void Motor(int vel){
  if(vel>255) vel = 255;
  if(vel<-255) vel = -255;

  if(vel>0){
    digitalWrite(IN3,HIGH);
    digitalWrite(IN4,LOW);
  }else if(vel<0){
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,HIGH);
  }else{
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,LOW);
  }
  analogWrite(SPD,abs(vel));
}

void get_serial_input() {
  char tmp;
  if (Serial.available() > 0) {
    tmp = Serial.read();
    if (tmp == '\n') {
      Serial.println(String(buffer));
      inputValue = buffer;
      buffer = 0;
    } else if (tmp >= '0' && tmp <= '9') {
      buffer = buffer * 10 + (tmp - '0');
    }
  }
}
