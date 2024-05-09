#ifndef MY_CONFIG_H
#define MY_CONFIG_H

#if defined(ESP8266) || defined(ESP32)

#ifndef DYNAMIC_JASON_DOC_SIZE
#define DYNAMIC_JASON_DOC_SIZE 2048
#endif

#include <Arduino.h>

#include "MyStringf.h"
#include <FS.h> //ESP8266:FS.h, ESP32:SPIFFS.h
#include <LittleFS.h>
#include <ArduinoJson.h> //https://github.com/bblanchon/ArduinoJson
//#include "ArduinoJson-6.x/src/ArduinoJson.h"

//#if defined(ESP32)
//#include <SPIFFS.h>
//#endif

class MyConfig {

public:
  MyConfig(const String& fileName);
  
  // set : save to Json SPIFFS
  void setString(const String& key,const String& val);
  void setInt(const String& key,int val);
  void setFloat(const String& key,float val);
  
  // get : read from Json SPIFFS
  String getString(const String& key);
  int getInt(const String& key);
  float getFloat(const String& key);

  // sync : read from Json SPIFFS, if not found save to Json SPIFFS
  void syncString(const String& key,String& initVal);
  void syncString(const char* key,char* initVal,int initValLen);
  void syncInt(const String& key,int& initVal);
  void syncFloat(const String& key,float& initVal);

  void json();                    // print JSON as pretty format
   
  void remove(const String& key); // remove JSON item
  void remove(const char* key); 
  void erase();                   // reset to empty JSON file

  // if key is exists or not 
  bool containsKey(const String& key);

  // ifEmptySet : if empty save to Json SPIFFS
  void ifEmptySetString(const String& key,const String& val);
  void ifEmptySetInt(const String& key,int val);
  void ifEmptySetFloat(const String& key,float val);

  void begin();  
  int format();
  int formatForce();
  void jsonPretty(Stream* myStream);
  void jsonPretty();

  void log(int level,const String& msg);
  void setLog(int logLevel);

private:
  int formatBase(bool mustFormat);
  void makeEmptyJsonFile();   
  String fileName;
  int logLevel = 3; // 1:detailed behavior, 2:major behavior, 3: none
  bool beforeBegin = true;
};

#endif // #if defined(ESP8266) || defined(ESP32)

#endif // #ifndef MY_CONFIG_H
