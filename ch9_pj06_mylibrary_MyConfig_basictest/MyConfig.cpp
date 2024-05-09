#if defined(ESP8266) || defined(ESP32)

#include "MyConfig.h"

//#if defined(ESP32)
//#define FILE_PREFIX "/LittleFS/"
//#elif defined(ESP8266)
//#define FILE_PREFIX ""
//#endif

#define FILE_PREFIX ""

namespace _MyConfig {
#include "MyGetChipId.h"
}

MyConfig::MyConfig(const String& INfileName) {
  fileName = String(FILE_PREFIX) + INfileName;
}

void MyConfig::log(int level,const String& msg) {
  if (level >= logLevel) {
    Serial.println(msg);
  }
}

void MyConfig::setLog(int INlogLevel) {
  logLevel = INlogLevel;
}

void MyConfig::begin() {
  beforeBegin = false;
  String msg = (char*)0;
  msg.reserve(63);

  log(1,F("MyConfig.begin > start"));
  format();
  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount failed");
    msg  = F("MyConfig.begin > LittleFS mount failed");
    log(2,msg);
    return;
  }

  File file = LittleFS.open(fileName, "r");
  if (!file) {
    makeEmptyJsonFile();
  }
  else {
    msg  = F("MyConfig.begin > file[");
    msg += fileName;
    msg += F("] exists");
    log(2,msg);
  }
  log(1,F("MyConfig.begin > end"));
}

int MyConfig::formatBase(bool mustFormat) {

  String msg((char*)0);
  msg.reserve(63);

  log(1,F("MyConfig.format > start"));
  if (mustFormat) {
    log(1,F("MyConfig.formatBase > mustFormat=true"));
   }
  else {
    log(1,F("MyConfig.formatBase > mustFormat=false"));
  }

  log(1,F("MyConfig.formatBase > before LittleFS.begin()"));  

  if (LittleFS.begin()) {
    log(1,F("MyConfig.formatBase > after LittleFS.begin() : ok"));
  }
  else {
    log(1,F("MyConfig.formatBase > after LittleFS.begin() : error"));
  }

  // make unique file name form ESP.MyGetChipId()
  String fileChipStr((char*)0);
  fileChipStr.reserve(32);

  //fileChipStr  = MyStringf("%08x",_MyConfig::MyGetChipId());
  char fileChip[33];
  sprintf(fileChip,"%08x",_MyConfig::MyGetChipId());  
  fileChipStr = String(fileChip) + F(".txt");

  fileChipStr = String(FILE_PREFIX) + fileChipStr;   
  
  msg  = "MyConfig.formatBase > (chipId)fileName:";
  msg += fileChipStr;
  log(1,msg);  

  if (!LittleFS.exists(fileChipStr) || mustFormat) { 
    log(2,F("MyConfig.formatBase > Please wait 30 secs for LittleFS to be formatted"));
    //LittleFS.format();
    //bool rtcd = LittleFS.format();
    bool rtcd = LittleFS.format();

    if (rtcd) {
      log(1,F("MyConfig.formatBase > after LittleFS.format() : ok"));
    }
    else {
      log(1,F("MyConfig.formatBase > after LittleFS.format() : error"));
    }

    log(2,F("MyConfig.formatBase > LittleFS formatted"));

    //File f = LittleFS.open(fileChipStr, "w");
    File f = LittleFS.open(fileChipStr, "w");
  
    if (f) {
      f.println(F("Format Complete")); 
      msg = F("MyConfig.formatBase > file[");
      msg += fileChipStr;
      msg += F("] created");
      log(2,msg);
      //MyConfigMakeFromNone();
    }
    else {
      msg  = F("MyConfig.formatBase > file[");
      msg += fileChipStr;
      msg += F("] open failed");
      log(2,msg);
    }
    makeEmptyJsonFile();
  } else {
    log(2,F("MyConfig.formatBase > LittleFS is already formatted."));
    msg  = F("MyConfig.formatBase > file[");
    msg += fileChipStr;
    msg += F("] exists");
    log(2,msg);
  } 
  log(1,F("MyConfig.formatBase > end"));

  return true;
}

int MyConfig::format() {
  bool mustFormat = false;
  formatBase(mustFormat);
  return true;
}

int MyConfig::formatForce() {
  bool mustFormat = true;
  formatBase(mustFormat);
  return true;
}

void MyConfig::makeEmptyJsonFile() {  
  log(1,F("MyConfig.makeEmptyJsonFile > start"));  
  DynamicJsonDocument docTemp(DYNAMIC_JASON_DOC_SIZE); 
  // to : create object
  // as : use object
  JsonObject root = docTemp.to<JsonObject>();

  if (root) {}

  //File file = LittleFS.open(fileName, "w");
  File file = LittleFS.open(fileName, "w");
  serializeJson(docTemp, file);
  file.close();   
  
  String msg((char*)0);
  msg.reserve(63);
  msg  = F("MyConfig.makeEmptyJsonFile > File [");
  msg += fileName;
  msg += F("] created");
  log(2,msg);
  log(1,F("MyConfig.makeEmptyJsonFile > end"));  
}

void MyConfig::erase() { 
  makeEmptyJsonFile(); 
}

void MyConfig::jsonPretty(Stream* myStream) {  
  if(beforeBegin) begin();

  DynamicJsonDocument docTemp(DYNAMIC_JASON_DOC_SIZE);  
  //File file = LittleFS.open(fileName, "r");
  File file = LittleFS.open(fileName, "r");
  DeserializationError error = deserializeJson(docTemp, file); 
  if (error) {}
  file.close(); 
  serializeJsonPretty(docTemp, *myStream);
  myStream->println(); 
}

void MyConfig::jsonPretty() {  
  jsonPretty(&Serial);
}

void MyConfig::json() {  
  jsonPretty(&Serial);
}

// set

void MyConfig::setString(const String& key,const String& val) {
  if(beforeBegin) begin();

  DynamicJsonDocument docTemp(DYNAMIC_JASON_DOC_SIZE);  
  //File file = LittleFS.open(fileName, "r");
  File file = LittleFS.open(fileName, "r");
  DeserializationError error = deserializeJson(docTemp, file); 
  if (error) {}
  file.close();  
  JsonObject root = docTemp.as<JsonObject>();
  
  root[key] = val;
    
  //file = LittleFS.open(fileName, "w");
  file = LittleFS.open(fileName, "w");
  serializeJson(docTemp, file); 
  file.close();

  char buf[64];
  snprintf(buf,sizeof(buf),"setString key=[%s],init=[%s]\n",key.c_str(),val.c_str()); 
  log(1,String(buf));

}

void MyConfig::setInt(const String& key,int val) {
  if(beforeBegin) begin();

  DynamicJsonDocument docTemp(DYNAMIC_JASON_DOC_SIZE);  
  //File file = LittleFS.open(fileName, "r");
  File file = LittleFS.open(fileName, "r");
  DeserializationError error = deserializeJson(docTemp, file); 
  if (error) {}
  file.close();  
  JsonObject root = docTemp.as<JsonObject>();
  
  root[key] = val;
    
  //file = LittleFS.open(fileName, "w");
  file = LittleFS.open(fileName, "w");
  serializeJson(docTemp, file); 
  file.close();
}

void MyConfig::setFloat(const String& key,float val) {
  if(beforeBegin) begin();

  DynamicJsonDocument docTemp(DYNAMIC_JASON_DOC_SIZE);  
  //File file = LittleFS.open(fileName, "r");
  File file = LittleFS.open(fileName, "r");
  DeserializationError error = deserializeJson(docTemp, file); 
  if (error) {}
  file.close();  
  JsonObject root = docTemp.as<JsonObject>();
  
  root[key] = val;
    
  //file = LittleFS.open(fileName, "w");
  file = LittleFS.open(fileName, "w");
  serializeJson(docTemp, file); 
  file.close();
}

// get

String MyConfig::getString(const String& key) {
  if(beforeBegin) begin();

  DynamicJsonDocument docTemp(DYNAMIC_JASON_DOC_SIZE);  
  //File file = LittleFS.open(fileName, "r");
  File file = LittleFS.open(fileName, "r");
  DeserializationError error = deserializeJson(docTemp, file); 
  if (error) {}
  file.close();  
  JsonObject root = docTemp.as<JsonObject>();  
  
  return root[key];
}

int MyConfig::getInt(const String& key) {
  if(beforeBegin) begin();

  DynamicJsonDocument docTemp(DYNAMIC_JASON_DOC_SIZE);  
  File file = LittleFS.open(fileName, "r");
  DeserializationError error = deserializeJson(docTemp, file); 
  if (error) {}
  file.close();  
  JsonObject root = docTemp.as<JsonObject>();  
  
  return root[key];
}

float MyConfig::getFloat(const String& key) {
  if(beforeBegin) begin();

  DynamicJsonDocument docTemp(DYNAMIC_JASON_DOC_SIZE);  
  File file = LittleFS.open(fileName, "r");
  DeserializationError error = deserializeJson(docTemp, file); 
  if (error) {}
  file.close();  
  JsonObject root = docTemp.as<JsonObject>();  

  return root[key];
}

// constainsKey

bool MyConfig::containsKey(const String& key) {
  if(beforeBegin) begin();

  DynamicJsonDocument docTemp(DYNAMIC_JASON_DOC_SIZE);  
  File file = LittleFS.open(fileName, "r");
  DeserializationError error = deserializeJson(docTemp, file); 
  if (error) {}
  file.close();  
  JsonObject root = docTemp.as<JsonObject>();
  if (root.containsKey(key)) {
    return true;
  }  
  else {
    return false;
  }
}

// if empty set

void MyConfig::ifEmptySetString(const String& key,const String& val) {
  if(beforeBegin) begin();

  if (!containsKey(key)) {
    setString(key,val);
  } 
}

void MyConfig::ifEmptySetInt(const String& key,int val) {
  if(beforeBegin) begin();

  if (!containsKey(key)) {
    setInt(key,val);
  } 
}

void MyConfig::ifEmptySetFloat(const String& key,float val) {
  if(beforeBegin) begin();

  if (!containsKey(key)) {
    setFloat(key,val);
  } 
}

// remove

void MyConfig::remove(const String& key) {
  if(beforeBegin) begin();

  DynamicJsonDocument docTemp(DYNAMIC_JASON_DOC_SIZE);  
  File file = LittleFS.open(fileName, "r");
  DeserializationError error = deserializeJson(docTemp, file); 
  if (error) {}
  file.close();  
  JsonObject root = docTemp.as<JsonObject>();
  root.remove(key); 
  file = LittleFS.open(fileName, "w");
  serializeJson(docTemp, file); 
  file.close();
}

void MyConfig::remove(const char* key) {
  remove(String(key));
}

// sync

void MyConfig::syncString(const String& key,String& initVal) {
  if(beforeBegin) begin();

  DynamicJsonDocument docTemp(DYNAMIC_JASON_DOC_SIZE);  
  File file = LittleFS.open(fileName, "r");
  DeserializationError error = deserializeJson(docTemp, file); 
  if (error) {}
  file.close();  
  JsonObject root = docTemp.as<JsonObject>();

  if(root.containsKey(key)) {
    initVal = getString(key);
  }
  else {
    setString(key,initVal);
  }
}

void MyConfig::syncString(const char* key,char* initVal,int initValSize) {
  if(beforeBegin) begin();
  
  char buf[64];
  snprintf(buf,sizeof(buf),"syncString key=[%s],init=[%s]",key,initVal); 
  log(1,String(buf));

  DynamicJsonDocument docTemp(DYNAMIC_JASON_DOC_SIZE);  
  File file = LittleFS.open(fileName, "r");
  DeserializationError error = deserializeJson(docTemp, file); 
  if (error) {}
  file.close();  
  JsonObject root = docTemp.as<JsonObject>();

  if(root.containsKey(key)) {
    strlcpy(initVal,getString(key).c_str(),initValSize);
  }
  else {
    setString(key,initVal);
  }
}

void MyConfig::syncInt(const String& key,int& initVal) {
  if(beforeBegin) begin();

  DynamicJsonDocument docTemp(DYNAMIC_JASON_DOC_SIZE);  
  File file = LittleFS.open(fileName, "r");
  DeserializationError error = deserializeJson(docTemp, file); 
  if (error) {}
  file.close();  
  JsonObject root = docTemp.as<JsonObject>();

  if(root.containsKey(key)) {
    initVal = getInt(key);
  }
  else {
    setInt(key,initVal);
  }
}

void MyConfig::syncFloat(const String& key,float& initVal) {
  if(beforeBegin) begin();

  DynamicJsonDocument docTemp(DYNAMIC_JASON_DOC_SIZE);  
  File file = LittleFS.open(fileName, "r");
  DeserializationError error = deserializeJson(docTemp, file); 
  if (error) {}
  file.close();  
  JsonObject root = docTemp.as<JsonObject>();

  if(root.containsKey(key)) {
    initVal = getFloat(key);
  }
  else {
    setFloat(key,initVal);
  }
}

#endif // #if defined(ESP8266) || defined(ESP32)
