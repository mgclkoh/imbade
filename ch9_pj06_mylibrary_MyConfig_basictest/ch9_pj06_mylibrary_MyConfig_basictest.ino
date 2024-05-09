#include <ArduinoJson.h>
#include "MyConfig.h"
#include "MySerial.h"
MyConfig myConfig("myConfig.json");
MySerial mySerial;

#define ALARM_VAL 300
int alarmVal = ALARM_VAL;

void setup() {
  Serial.begin(115200);
  Serial.println();

  Serial.println("Iput > 'alarm nnn' to change alarmVal");
  Serial.println("Iput > 'erase' to erase JSON file");
  Serial.println("Iput > 'json' to print JSON file");

  Serial.printf("Initial alarmVal=%d\n",alarmVal);
  myConfig.syncInt("alarm",alarmVal);
  Serial.printf("JSON alarmVal=%d\n",alarmVal);

  mySerial.begin(mySerialCallback);
}

void loop() {
  mySerial.run();
}

void mySerialCallback(const String& key,const String& val) {
  if (key == "alarm") {
    myConfig.setInt("alarm",val.toInt());
    alarmVal = myConfig.getInt("alarm");
    Serial.printf("alarmVal=%d\n",alarmVal);
  }
  else if (key == "erase") {
    myConfig.erase(); // JSON 파일 비우기
  }
  else if (key == "json") {
    myConfig.json();  // JSON 파일을 보기 좋게 인쇄
  } 
}
