#include "MyOutput.h"
#include "MySerial.h"

#define LED_PIN LED_BUILTIN
#define LED_ON  LOW         //current sinking

MyOutput led(LED_PIN,LED_ON);
MySerial mySerial;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Input >> on|off|toggle|set|set2|set3|status");
  mySerial.begin(mySerialCallback);
}

void loop() {
  led.run();
  mySerial.run();
}

void mySerialCallback(const String& key, const String& val) {
  if (key == "on") {
    led.on();
  }
  else if (key == "off") {
    led.off();
  }
  else if (key == "toggle") {
    led.toggle();
  }
  else if (key == "set") {
    led.set(300,200);
  }
  else if (key == "set2") {
    led.set(300,200,3);
  }
  else if (key == "set3") {
    led.set(300,200,3,5,1000);
  }
  else if (key == "status") {
    Serial.printf("led.status() : %d\n",led.status());
  }
}
