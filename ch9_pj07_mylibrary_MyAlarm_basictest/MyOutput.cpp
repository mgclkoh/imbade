#include <Arduino.h>
#include "MyOutput.h"

/*****
constructor
*****/  

MyOutput::MyOutput(int INpinNo,int INonVal) {
  
  init(INpinNo,INonVal);
  
}

MyOutput::MyOutput(int INpinNo) {
  
  init(INpinNo,HIGH);
  
}

MyOutput::MyOutput() {
  
  init(255,HIGH);
  
}

void MyOutput::setPin(int INpinNo,int INonVal) {
  
  init(INpinNo,INonVal);
  
}

void MyOutput::setPin(int INpinNo) {
  
  init(INpinNo,HIGH);
  
}

/*****
base for constructor
*****/

void MyOutput::init(int INpinNo,int INonVal) {
  
  pinNo = INpinNo;
  onVal = INonVal;
  offVal = !onVal;
  
  runStage = STAGE_IDLE;

  pinMode(pinNo,OUTPUT);
  digitalWrite(pinNo,offVal);
  pinStatus = STATUS_OFF;
  SVpinStatus = pinStatus;
  
}

/*****
set
*****/

void MyOutput::setBase(unsigned long INonMil,unsigned long INoffMil,int INonOffCnt,int INonOffCycle,unsigned long INonOffCycleDelayMil) {
  
  runModifyOn = true;
  
  onMil = INonMil;
  offMil = INoffMil;
  onOffCnt = INonOffCnt;
  onOffCycle = INonOffCycle;
  onOffCycleDelayMil = INonOffCycleDelayMil;
  
  
  
  if (onOffCnt == -1) {
    onOffCntForever = true;
    RunOnOffCnt = 1;
  }
  else { 
    onOffCntForever = false;
    RunOnOffCnt = onOffCnt;
  }
  
  if (onOffCycle == -1) {
    onOffCycleForever = true;
    RunOnOffCycle = 1;
  }
  else {
    onOffCycleForever = false;
    RunOnOffCycle = onOffCycle;
  }
  
  off();
  
  pinStatus = STATUS_BLINK; 
  runStage = STAGE_INIT; 
  
  runModifyOn = false;
}

void MyOutput::set(unsigned long INonMil,unsigned long INoffMil,int INonOffCnt, 
                    int INonOffCycle,unsigned long INonOffCycleDelayMil) {
             
  setBase(INonMil,INoffMil,INonOffCnt,INonOffCycle,INonOffCycleDelayMil);
}

void MyOutput::set(unsigned long INonMil,unsigned long INoffMil,int INonOffCnt, 
                    int INonOffCycle) {

  unsigned long onOffCycleDelayMil = 1000;       
  setBase(INonMil,INoffMil,INonOffCnt,INonOffCycle,onOffCycleDelayMil);
}


void MyOutput::set(unsigned long INonMil,unsigned long INoffMil,int INonOffCnt) {
  
  int INonOffCycle = 1;
  unsigned long INonOffCycleDelayMil = 1000;
  
  setBase(INonMil,INoffMil,INonOffCnt,INonOffCycle,INonOffCycleDelayMil);
}

void MyOutput::set(unsigned long INonMil,unsigned long INoffMil) {
  
  int INonOffCycle = 1;
  int INonOffCnt = -1;

  unsigned long INonOffCycleDelayMil = 1000;
  
  setBase(INonMil,INoffMil,INonOffCnt,INonOffCycle,INonOffCycleDelayMil);
}

//put in loop() rather than outputISR
void MyOutput::run() {
  
  if (runModifyOn)
    return;

  //pinStatus = STATUS_OFF;
  //digitalWrite(pinNo,offVal);
  
  if (pinStatus == STATUS_OFF || pinStatus == STATUS_ON ) 
    return;
  
  pinCmnd = CMND_IDLE;
  thisMil = millis();
  
  switch(runStage) {
    case STAGE_IDLE:
      return;
      break;
    case STAGE_INIT:
      runStage = STAGE_ON;
      onOffCntDown = RunOnOffCnt;     
      onOffCycleDown = RunOnOffCycle;

      // if at first onMil == 0 off on init
      if (onOffCntDown == 1 && onMil == 0) {
        pinCmnd = CMND_OFF;
      }
      else {
        pinCmnd = CMND_ON;
      }

      nextMil = thisMil + onMil;
      break;  
    case STAGE_ON:
      if (thisMil >= nextMil) {
        runStage = STAGE_OFF;
        pinCmnd = CMND_OFF;
        nextMil = thisMil + offMil;       
      }
      break;  
    
    case STAGE_OFF: 
      if (thisMil >= nextMil) {
        
        onOffCntDown--;
        if (onOffCntForever) 
          onOffCntDown++;
        
        if (onOffCntDown > 0) {
          runStage = STAGE_ON;
          pinCmnd = CMND_ON;
          nextMil = thisMil + onMil;            
        }
        else {
          runStage = STAGE_CYCLE_DELAY;
          nextMil = thisMil + onOffCycleDelayMil; 
        }
      }
      break;  
    case STAGE_CYCLE_DELAY:
      if (thisMil >= nextMil) {
        
        onOffCycleDown--;
        if (onOffCycleForever) 
          onOffCycleDown++;
        
        if (onOffCycleDown > 0) {
          runStage = STAGE_ON;
          onOffCntDown = RunOnOffCnt;
          pinCmnd = CMND_ON;
          nextMil = thisMil + onMil;
        }
        else {
          runStage = STAGE_IDLE;
          if (setEnd) {
            pinCmnd = CMND_ON;
            pinStatus = STATUS_ON;
            setEnd = false; // 2020.02.18
          }
          else {
            pinCmnd = CMND_OFF;
            pinStatus = STATUS_OFF;
          }
        }
      }
      break;
  }

  switch (pinCmnd) {
    case CMND_ON: digitalWrite(pinNo,onVal); break;
    case CMND_OFF:  digitalWrite(pinNo,offVal); break;
  }

  #if SAN_OUTPUT_DELAY_USE
  delay(SAN_OUTPUT_DELAY_MIL);
  #endif
}

/*****
setEndOn(),setEndOff()
*****/

void MyOutput::setEndOn() {
  setEnd = true;
}

void MyOutput::setEndOff() {
  setEnd = false;
}

/*****
on(),off(),set(),toggle()
*****/

void MyOutput::on() {
  pinStatus = STATUS_ON;
  digitalWrite(pinNo,onVal);
}

void MyOutput::off() {
  pinStatus = STATUS_OFF;
  digitalWrite(pinNo,offVal);
}

void MyOutput::on(unsigned long onMil) {
  setEndOff();
  set(onMil,0,1);
}

void MyOutput::off(unsigned long offMil) {
  setEndOn();
  set(0,offMil,1);
}

void MyOutput::set(int INonOffStatus) {

  switch(INonOffStatus) {
    case 0: off();  break;
    case 1: on(); break;
  }
    
}

void MyOutput::set(const char* INonOffStatus) {
  String payload = INonOffStatus;
  set(payload);
}

void MyOutput::set(const String& INonOffStatus) {
  
  String payload = INonOffStatus;

  payload.toLowerCase();
  payload.trim();
    
    if (toBool(payload)) {
      on();
    }
    else {
      off();
    }
}

int MyOutput::toBool(const String& INpayload) {

  int rtcd = false;
  String payload = INpayload;
  payload.toLowerCase();
  
  if (payload == "0" || payload == "off" || payload == "false" || payload == "no" || payload == "n" ) {
    rtcd = false;
  } else if (payload == "1" || payload == "on" || payload == "true" || payload == "yes" || payload == "y") {  
    rtcd = true;
  }
  return rtcd;
} 

void MyOutput::toggle() {

  if (pinStatus == STATUS_ON || pinStatus == STATUS_BLINK ) {
    SVpinStatus = pinStatus;
    pinStatus = STATUS_OFF;   
  }
  else if (SVpinStatus == STATUS_BLINK ) {
    pinStatus = SVpinStatus;
  }
  else {
    pinStatus = STATUS_ON;  
  }
  
  if (pinStatus == STATUS_ON || pinStatus == STATUS_OFF ) {
    set(pinStatus);
  }
}

/*****
pinStatus(), pinVal(), onBlink()
*****/

int MyOutput::status3() {
  return pinStatus;
}

int MyOutput::status() {
  if (pinStatus == STATUS_OFF) {
    return false;
  }
  else {
    return true;
  }
}

String MyOutput::status3(const String& onStr, const String& offStr,
                            const String& blinkStr) {
  if (pinStatus == STATUS_OFF) {
    return offStr;
  }
  else if (pinStatus == STATUS_ON) {
    return onStr;
  }
  else if (pinStatus == STATUS_BLINK) {
    return blinkStr;
  }
  else {
    return String(pinStatus);  // error
  }
}

String MyOutput::status(const String& onStr, const String& offStr) {
  if (pinStatus == STATUS_OFF) {
    return offStr;
  }
  else if (pinStatus == STATUS_ON || pinStatus == STATUS_BLINK ) {
    return onStr;
  }
  else {
    return String(pinStatus);
  }
}

int MyOutput::val() {
  return digitalRead(pinNo);
}

bool MyOutput::isOn() {
  if (pinStatus == STATUS_ON || pinStatus == STATUS_BLINK) {
    return true;
  }
  else {
    return false;
  }
}

bool MyOutput::isOff() {
  if (pinStatus == STATUS_OFF) {
    return true;
  }
  else {
    return false;
  }
}
bool MyOutput::isBlink() {
  if (pinStatus == STATUS_BLINK) {
    return true;
  }
  else {
    return false;
  }
}
