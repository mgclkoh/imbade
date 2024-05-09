#include "MyAlarm.h"
#include "MyOutput.h"

#define MOTION_PIN D3
#define LED_PIN LED_BUILTIN
#define MOTION_ON_SEC 10

MyAlarm  motion(MOTION_PIN,LOW,INPUT_PULLUP,MOTION_ON_SEC); //Alarm 발생 객체
MyOutput led(LED_PIN,LOW); //Alarm 출력 객체

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Press button for alarm on/off");
}

void loop() {
  led.run();
  if (motion.toOn()) {
    led.set(700,300);
    Serial.println(">>Motion On");
  }
  else if(motion.toOff()) {
    led.off();
    Serial.println(">>Motion Off");
  }
}
