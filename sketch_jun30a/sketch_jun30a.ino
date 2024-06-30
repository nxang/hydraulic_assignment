#include "MovingAverageFloat.h"
#define pin1 8
#define pin2 17
#define pin3 18
#define button1 38
#define button2 37
#define button3 36
#define trigger 11
#define echo 10

#define PIN1(state) digitalWrite(pin1, state)
#define PIN2(state) digitalWrite(pin2, state)
#define PIN3(state) digitalWrite(pin3, state)
#define BUTTON1 digitalRead(button1)
#define BUTTON2 digitalRead(button2)
#define BUTTON3 digitalRead(button3)


#define RGB 97
#define RGB_TRANSITION_TIME 100  // Transition time in milliseconds (5 seconds)
#define BRIGHTNESS 10
MovingAverageFloat <16> filter;
float duration, distance, target, dist, error;


void MainTask(void *pvParameters);
void RGB_Task(void *pvParameters);
void Sensor_Task(void *pvParameters);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);

  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  Serial.print("???");
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

  xTaskCreate(
    RGB_Task, "RGB Task",
    20480,
    NULL,
    3,
    NULL);
}
/*
01:extend
10:retract
*/
void loop() {
}

void MainTask(void *pvParameters) {
  // target = 7.0;
  for (;;) {

    // put your main code here, to run repeatedly:
    if (BUTTON1) {
      neopixelWrite(RGB, 10, 0, 0);
      target = 7.0;
      // PIN2(0);
      // PIN1(1);
      // Serial.println("button1");
    } else if (BUTTON2) {
      neopixelWrite(RGB, 0, 10, 0);
      target = 15.0;
      // PIN2(1);
      // PIN1(0);
    } else if (BUTTON3) {
      neopixelWrite(RGB, 0, 0, 10);
      target = 23.0;

      // Serial.println("button3");
    } else {
      neopixelWrite(RGB, 10, 10, 10);

    }
    error = target - dist;
    if(error>1.0){
      PIN2(1);
      PIN1(0);
    }else if(error<-1.0){
      PIN2(0);
      PIN1(1);
    }else{
      PIN2(0);
      PIN1(0);
    }
    // Serial.print(dist);
    //   Serial.println(target);
    // PIN1(1);
    // PIN2(0);
    // PIN3(0);
    // vTaskDelay(50);
    // PIN2(1);
    // PIN3(0);
    vTaskDelay(1);
  }
}

void Sensor_Task(void *pvParameters) {

  for (;;) {
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);

  duration = pulseIn(echo, HIGH);
  distance = (duration*.0343)/2;
  filter.add(distance);
  dist = filter.get();
  Serial.print("Distance: ");
  Serial.print(dist);
  Serial.print(" Target: ");
  Serial.println(target);
  
  // delay(100);
    vTaskDelay(1);
  }
}

void RGB_Task(void *pvParameters) {

  for (;;) {
    // PIN2(0);
    //   PIN3(1);
    vTaskDelay(1);
  }
}
