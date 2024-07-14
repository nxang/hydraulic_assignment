#include "MovingAverageFloat.h"
#define pin1 8
#define pin2 17
#define button1 38
#define button2 37
#define button3 36
#define trigger 11
#define echo 10

#define PIN1(state) digitalWrite(pin1, state)
#define PIN2(state) digitalWrite(pin2, state)
#define BUTTON1 digitalRead(button1)
#define BUTTON2 digitalRead(button2)
#define BUTTON3 digitalRead(button3)

MovingAverageFloat <16> filter;
float duration, distance, target, dist, error;

void MainTask(void *pvParameters);
void Sensor_Task(void *pvParameters);

void setup() {
  //Initilize
  Serial.begin(115200);
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);

  //Create freertos task
  xTaskCreate(
    MainTask, "Main Task",
    20480,
    NULL,
    2,
    NULL);

  xTaskCreate(
    Sensor_Task, "Sensor Task",
    20480,
    NULL,
    1,
    NULL);
}
/*
01:extend
10:retract
*/
void loop() {
}

void MainTask(void *pvParameters) {
  for (;;) {
    //check for button state to determine target position
    if (BUTTON1) {
      neopixelWrite(RGB, 10, 0, 0);
      target = 7.0;
    } else if (BUTTON2) {
      neopixelWrite(RGB, 0, 10, 0);
      target = 15.0;
    } else if (BUTTON3) {
      neopixelWrite(RGB, 0, 0, 10);
      target = 23.0;
    } else {
      neopixelWrite(RGB, 10, 10, 10);
    }
    //Extend or retract to reach the target
    error = target - dist;
    if(error>1.0){
      PIN1(0);
      PIN2(1);
    }else if(error<-1.0){
      PIN1(1);
      PIN2(0);
    }else{
      PIN2(0);
      PIN1(0);
    }
    vTaskDelay(1);
  }
}

void Sensor_Task(void *pvParameters) {

  for (;;) {
  //Get distance with ultrasonic sensor
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  duration = pulseIn(echo, HIGH);
  distance = (duration*.0343)/2;

  //Apply moving average to get stable value
  filter.add(distance);
  dist = filter.get();

  vTaskDelay(1);
  }
}
