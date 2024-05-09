#include <Arduino.h>
#include "MySerial.h"

// ------------------------------------------------
// constructor

MySerial::MySerial() {
  this->myStream = &Serial;
}

MySerial::MySerial(void(*INcallback)(const String& key,const String& val) ) {
  this->myStream = &Serial;
  this->callback = INcallback;
}

MySerial::MySerial(Stream* INmyStream,void(*INcallback)(const String& key,const String& val)) {
  this->myStream = INmyStream;
  this->callback = INcallback;  

  if (this->myStream != &Serial) {
    isEchoOn = false;
  }  
}

MySerial::MySerial(void(*INcallback)(String key,String val) ) {
  this->myStream = &Serial;
  this->callbackCopy = INcallback;
  cbType = 1; // copy
}

MySerial::MySerial(Stream* INmyStream,void(*INcallback)(String key,String val)) {
  this->myStream = INmyStream;
  this->callbackCopy = INcallback;  
  cbType = 1; // copy

  if (this->myStream != &Serial) {
    isEchoOn = false;
  }  
}

// ------------------------------------------------
// setup

void MySerial::begin() {

}

void MySerial::begin(void(*INcallback)(const String& key,const String& val) ) {
  this->callback = INcallback;
}

void MySerial::begin(Stream* INmyStream,void(*INcallback)(const String& key,const String& val)) {
  this->myStream = INmyStream;
  this->callback = INcallback;

  if (this->myStream != &Serial) {
    isEchoOn = false;
  }
}

void MySerial::begin(void(*INcallback)(String key,String val) ) {
  this->callbackCopy = INcallback;
  cbType = 1; // copy
}

void MySerial::begin(Stream* INmyStream,void(*INcallback)(String key,String val)) {
  this->myStream = INmyStream;
  this->callbackCopy = INcallback;
  cbType = 1; // copy

  if (this->myStream != &Serial) {
    isEchoOn = false;
  }
}

// ------------------------------------------------
// Check Serial.available() and invoke callback.

void MySerial::run() {

  if (myStream->available() == 0) {
    return;
  }

  String rcvStr((char*)0);
    
  // To prevent heap shortage by means of .reserve()
  // Hangeul:
  // object.reserve()로 최대길이를 미리 정하여
  // heap 메모리 부족을 방지한다.

  rcvStr.reserve(SAN_SERIAL_STR_LEN);

  while(myStream->available()) {
    char c = myStream->read();
    if (c == ';') {
      break;
    }
    rcvStr += c;

    if (rcvStr.length() >= SAN_SERIAL_STR_LEN) {
      break;
    }

    // For slow SoftSerial to receive data.
    delay(serialDelayMil);
  }

  String keyStr((char*)0);
  String valStr((char*)0);

int indexFrom;

  rcvStr.trim();
  indexFrom = 0;
  
  int blankIndx = rcvStr.indexOf(' ',indexFrom);  
  // -1: index not found
  if (blankIndx == -1) {
    keyStr = rcvStr;
    valStr = "";
  }
  else {
    keyStr = rcvStr.substring(0,blankIndx);
    valStr = rcvStr.substring(blankIndx+1);
  }

  keyStr.trim();
  valStr.trim();
  
  if (isEchoOn) {
    myStream->print("INP> key=[");
    myStream->print(keyStr);
    myStream->print("],val=[");
    myStream->print(valStr);
    myStream->print("]\n");
  }
  if (cbType == 0 ) {
    callback(keyStr, valStr);
  }
  else if (cbType == 1 ) {
    callbackCopy(keyStr, valStr);
  }

  return;
}
int MySerial::verifyBool(const String& INpayload) {
  String payload = INpayload;
  payload.toLowerCase();
  if (payload == "0"   || payload == "off" || payload == "false" ||
      payload == "no" || payload == "n" ) {
    return false; 
  }
  else if (payload == "1" || payload == "on" || payload == "true" ||
      payload == "yes" || payload == "y") {
    return true;  
  } 
  else {
    return -1;
  }
}  

bool MySerial::toBool(const String& INpayload) {
  return verifyBool(INpayload);
}  

void MySerial::echoOn() {
  isEchoOn = true;
}

void MySerial::echoOff() {
  isEchoOn = false;
}



  
