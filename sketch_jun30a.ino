#define pin1 8
#define pin2 17
#define pin3 18
#define button 5
#define trigger 11
#define echo 10

#define PIN1(state) digitalWrite(pin1, state)
#define PIN2(state) digitalWrite(pin2, state)
#define PIN3(state) digitalWrite(pin3, state)
#define BUTTON analogRead(button)

#define RGB 97
#define RGB_TRANSITION_TIME 100  // Transition time in milliseconds (5 seconds)
#define BRIGHTNESS 10
float duration, distance;

void MainTask(void *pvParameters);
void RGB_Task(void *pvParameters);
void Sensor_Task(void *pvParameters);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
    pinMode(5, INPUT_PULLDOWN);

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
  for (;;) {

    // put your main code here, to run repeatedly:
    if (BUTTON < 1200) {
      neopixelWrite(RGB, 0, 0, 0);

      Serial.println("no press");
    } else if (BUTTON > 1300 && BUTTON < 1900) {
      neopixelWrite(RGB, 10, 0, 0);
      // Serial.println("button1");
    } else if (BUTTON > 1950 && BUTTON < 2700) {
      neopixelWrite(RGB, 0, 10, 0);
    } else if (BUTTON > 2750 && BUTTON < 3600) {
      neopixelWrite(RGB, 0, 0, 10);

      // Serial.println("button3");
    } else {
      neopixelWrite(RGB, 10, 10, 10);

    }
      // Serial.println(BUTTON);
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
  Serial.print("Distance: ");
  Serial.println(distance);
  // delay(100);
    vTaskDelay(1);
  }
}

void RGB_Task(void *pvParameters) {

  for (;;) {
    vTaskDelay(10);
  }
}
