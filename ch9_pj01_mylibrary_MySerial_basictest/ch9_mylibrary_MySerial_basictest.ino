#include "MySerial.h"
MySerial mySerial;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Input > name|ssid|count|temp val");
  mySerial.begin(mySerialCallback);
}

void loop() {
  mySerial.run();
}

void mySerialCallback(const String& key, const String& val) {
  if (key == "name") {
    // String
    String nm = val;
    nm.toUpperCase(); // error: val.toUpperVase();
    Serial.printf(">>name=%s\n",nm.c_str());
  }
  else if (key == "ssid") {
    // char[]
    char ssid[20];
    val.toCharArray(ssid,sizeof(ssid));
    Serial.printf(">>ssid=%s\n",ssid);
  }
  else if (key == "count") {
    // int
    int cnt = val.toInt();
    Serial.printf(">>count=%d\n",cnt);
  }
  else if (key == "temp") {
    // float
    float t = val.toFloat();
    Serial.printf(">>temp=%.2f\n",t);
  }
}
