#include "ext_servo.h"
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
ExtServo servo_1;

// void test_task_1(void* pvParameters);
// void test_task_2(void* pvParameters);

// static void MyTask1(void* pvParameters);
// static void MyTask2(void* pvParameters);
// static void MyIdleTask(void* pvParameters);

void setup() {
  Serial.begin(9600);
  servo_1.ext_servo_connect(9, 0, 180, 0);
  servo_1.ext_servo_set(0, 100);
  // servo_1.ext_servo_resume();

  // Example of creating a task
  // xTaskCreate(MyTask1, "Task1", 100, NULL, 1, NULL);
  // xTaskCreate(MyTask2, "Task2", 100, NULL, 2, NULL);
  // xTaskCreate(MyIdleTask, "IdleTask", 100, NULL, 0, NULL);
}

void loop() {
}

// Example of creating a task
// /* Task1 with priority 1 */
// static void MyTask1(void* pvParameters)
// {
//   while(1)
//   {
//     Serial.println(F("Task1"));
//     vTaskDelay(100/portTICK_PERIOD_MS);
//   }
// }

// /* Task2 with priority 2 */
// static void MyTask2(void* pvParameters)
// {
//   while(1)
//   {    
//     Serial.println(F("Task2"));
//     vTaskDelay(150/portTICK_PERIOD_MS);
//   }
// }

// /* Idle Task with priority Zero */ 
// static void MyIdleTask(void* pvParameters)
// {
//   while(1)
//   {
//     Serial.println(F("Idle state"));
//     delay(50);
//   }
// }