#include <Arduino.h>
#include "MyAlarm.h"

// digital pin
MyAlarm::MyAlarm(uint8_t INpinNo, int INactiveVal, int INinputMode, float INalarmDurationSec) {
  pinNo   = INpinNo;
  inputMode = INinputMode;
  activeVal= INactiveVal;
  alarmDurationSec = INalarmDurationSec;
  alarmDurationMil= alarmDurationSec * 1000.0;

  pinRead   = !activeVal;
  SVpinRead = !activeVal;

  alarmOnUntilMil = 0;

  digitalOrAnalog = ALARM_DIGITAL;
  alarmOn = false;
  startOn = true;
  toOnManual = false;
  toOffManual = false;

  isToStart = false;
  isToStop  = false;
  
  pinMode(pinNo,inputMode);
}

// alalog pin
MyAlarm::MyAlarm(uint8_t INpinNo, int INtrigger, float INalarmDurationSec) {
  pinNo   = INpinNo;
  triggerVal = INtrigger;

  alarmDurationSec = INalarmDurationSec;
  alarmDurationMil= alarmDurationSec * 1000.0;

  alarmOnUntilMil = 0;

  digitalOrAnalog = ALARM_ANALOG;
  alarmOn = false;
  startOn = true;
  
}

bool MyAlarm::toOn() {
  if (digitalOrAnalog == ALARM_DIGITAL) {
    return toOnDigital();
  }
  else {
    return toOnAnalog();
  }
}

bool MyAlarm::toOff() {
  if (digitalOrAnalog == ALARM_DIGITAL) {
    return toOffDigital();
  }
  else {
    return toOffAnalog();
  }
}

void MyAlarm::on() {
  alarmOn = true;

  toOnManual = true;
  toOffManual = false;

  alarmOnUntilMil = millis() + alarmDurationMil; 
}

void MyAlarm::off() {
   alarmOn = false;

   toOnManual = false;
   toOffManual = true;
}

void MyAlarm::reset() {
   alarmOn = false;

   toOnManual = false;
   toOffManual = false;
}

bool MyAlarm::toOnDigital() {
  static unsigned long onStartMil = 0;
  if (toOnManual) {
    toOnManual = false;
    onStartMil = millis();
    return true;
  }

  if (!startOn) {
    return false;
  }

  pinRead = digitalRead(pinNo);
  if (!alarmOn && pinRead == activeVal) {
    alarmOn = true; 
    alarmOnUntilMil = millis() + alarmDurationMil;
    onStartMil = millis();
    return true;   
  } 
  else if (alarmOn && pinRead == activeVal) {
    alarmOn = true;
    // extend  alarmOnUntilMil after 2 seconds 
    if (millis() - onStartMil > 2000 ) {
      // to get second boundary value
      alarmOnUntilMil = onStartMil + (millis() - onStartMil)/1000*1000 + alarmDurationMil;
    }
    return false;   
  } 
  else {
    return false;
  } 
}

bool MyAlarm::toOffDigital() {
  if (toOffManual) {
    toOffManual = false;
    alarmOn = false;
    return true;
  }

  if (!startOn) {
    return false;
  }

  pinRead = digitalRead(pinNo);
  //delay(0);
  if (alarmOn && pinRead == !activeVal && millis() > alarmOnUntilMil) {
    alarmOn = false;
    delay(20);
    return true;   
  }
  else {
    return false;
  } 
}

bool MyAlarm::toOnAnalog() {
  if (toOnManual) {
    toOnManual = false;
    return true;
  }

  if (!startOn) {
    return false;
  }

  pinRead = analogRead(pinNo); 
  delay(0);

  bool condition;
  if (triggerVal > 0) {
    condition = (pinRead >= triggerVal);
  }
  else {
    condition = (pinRead <= triggerVal*(-1));
  }

  if (!alarmOn && condition) {
    alarmOn = true;
    alarmOnUntilMil = millis() + alarmDurationMil; 
    return true;       
  } 
  else if (alarmOn && condition) {
    alarmOn = true;
    alarmOnUntilMil = millis() + alarmDurationMil; 
    return false;       
  } 
  else {
    return false;
  }
}     
 
bool MyAlarm::toOffAnalog() {
  if (toOffManual) {
    toOffManual = false;
    alarmOn = false;
    return true;
  }

  if (!startOn) {
    return false;
  }

  pinRead = analogRead(pinNo); 
  delay(0);

  bool condition;
  if (triggerVal > 0) {
    condition = (pinRead >= triggerVal);
  }
  else {
    condition = (pinRead <= triggerVal*(-1));
  }

  if (alarmOn && !condition && millis() > alarmOnUntilMil) {
    alarmOn = false;
    return true;       
  } 
  else {
    return false;
  } 
}

bool MyAlarm::isOn() {
  return alarmOn;
}

bool MyAlarm::isOff() {
  return !alarmOn;
}

int MyAlarm::status() {
  return alarmOn;
}

String MyAlarm::status(const String& onStr,const String& offStr) {

  if (alarmOn) {
    return onStr;
  }
  else {
    return offStr;
  }
}

void MyAlarm::start() {
  reset();
  startOn = true;
  isToStart = true;
}

void MyAlarm::stop() {
  if(isOn()) {
    off();
  }
  startOn = false;
  isToStop  = true;
}

void MyAlarm::toggleStartStop() {
  if(isStart()) {
    stop();
  }
  else {
    start();
  }
}

bool MyAlarm::toStart() {
  bool SVisToStart = isToStart;
  isToStart = false;
  return SVisToStart;
}

bool MyAlarm::toStop() {
  bool SVisToStop = isToStop;
  isToStop = false;
  return SVisToStop;
}

bool MyAlarm::isStart() {
  return startOn;
}

bool MyAlarm::isStop() {
  return !startOn;
}

int MyAlarm::read() {
  if (digitalOrAnalog == ALARM_DIGITAL) {
    pinRead = digitalRead(pinNo);
  } 
  else {
    pinRead = analogRead(pinNo);
  }
  return pinRead;
}

void MyAlarm::setTrigger(int INtrigger) {
  triggerVal = INtrigger;
}

int MyAlarm::getTrigger() {
  return triggerVal;
}

void MyAlarm::setDuration(float INalarmDurationSec) {
  alarmDurationSec = INalarmDurationSec;
  alarmDurationMil= alarmDurationSec * 1000.0;
}

float MyAlarm::getDuration() {
  return alarmDurationSec;
}

float MyAlarm::getRemain() {
  float remainSec;
  if (alarmOn) {
    remainSec = (alarmOnUntilMil - millis()) / 1000.0;
    //Serial.printf("getRemain mil=%u,until=%u",millis(),alarmOnUntilMil);
    if (remainSec < 0) {
      remainSec = 0;
    }
  }
  else {
    remainSec = 0;
  }

  return remainSec;
}

void MyAlarm::run() {
  ;
}

void MyAlarm::setSyncLight(int tORf) {
  if (_isSyncLight && alarmOn) {
    off();
  }
  _wasSyncLight = _isSyncLight;
  _isSyncLight = tORf;
}

bool MyAlarm::isSyncLight() {
  return _isSyncLight;
}

bool MyAlarm::wasSyncLight() {
  bool tempSyncLight = _wasSyncLight;
  _wasSyncLight = _isSyncLight;

  return tempSyncLight;
}

void MyAlarm::setReportAlarm(int tORf) {
  if (_isReportAlarm && alarmOn) {
    off();
  }
  _wasReportAlarm = _isReportAlarm;
  _isReportAlarm = tORf;
}

bool MyAlarm::isReportAlarm() {
  return _isReportAlarm;
}

bool MyAlarm::wasReportAlarm() {
  bool tempReportAlarm = _wasReportAlarm;
  _wasReportAlarm = _isReportAlarm;

  return tempReportAlarm;
}
