#define ALARM_ON_SEC 5.0

#include "MySerial.h"
#include "MyTimer.h"
#include "MyOutput.h"
#include "MySandglass.h"

MySerial mySerial;
MyTimer timerRemain(1);
MyOutput led(LED_BUILTIN,LOW);
MySandglass sandLed(ALARM_ON_SEC);

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Input 'on|off|dur'");
  Serial.println("Input 'set 5' for 5 durationSec ");

  mySerial.begin(mySerialCallback);
}

void loop() {
  led.run();
  mySerial.run();
 
  if (sandLed.toOn()) {
    led.set(700,300);
    Serial.printf("sandLed : ON\n");
  }
  else if (sandLed.toOff()) {
    led.off();
    Serial.printf("sandLed : OFF\n");
  }

  if (timerRemain.isOn()) {
    if (sandLed.isOn()) {
      Serial.printf("remaining ... %.0f\n",
                       sandLed.getRemain());
    }  
  }
}

void mySerialCallback(const String& key,const String& val){
  if (key == "on") {
    sandLed.on();
  }
  else if (key == "off") {
    sandLed.off();
  }  
  else if (key == "set") {
    sandLed.setDuration(val.toFloat());
    Serial.printf("durationSec=%.0f\n",sandLed.getDuration());
  }
  else if (key == "dur") {
    Serial.printf("durationSec=%.01f\n",sandLed.getDuration());
  }
}
