#include <Arduino.h>
#include "MyTimer.h"

void MyTimer::MyTimerBase(double INintervalSec) {
  intervalMil = INintervalSec * 1000;
  SVintervalMil = intervalMil;
  fst = true;
}

// constructor
MyTimer::MyTimer() {
  MyTimerBase(TIMER_INTERVAL_SEC);
}

MyTimer::MyTimer(double INintervalSec) {
  MyTimerBase(INintervalSec);
}

void MyTimer::set(double INintervalSec) {
  intervalMil = INintervalSec * 1000;
  SVintervalMil = intervalMil;
  nextMil = millis() + intervalMil;
}

void MyTimer::reset() {
  SVintervalMil = intervalMil;
  nextMil = millis() + intervalMil;
}

void MyTimer::nowOn() {
  SVintervalMil = intervalMil;
  nextMil = 0; 
  fst = false;
}

bool MyTimer::isOn() {
  
  bool rtcd = false;

  if ( intervalMil == 0 ) {
    return false;
  }

  
  if ( nextMil == 0 || millis() > nextMil) {
    nextMil = millis() + intervalMil;
    if (fst) {
      fst = false;
      rtcd = false;
    }
    else {
      rtcd = true;
    }
    rtcd = true;
  }
  else {
    rtcd = false;
  }
  
  return rtcd;
}

bool MyTimer::isOn(double INintervalSec) {
  unsigned long INintervalMil = INintervalSec * 1000;
  if (SVintervalMil != INintervalMil) {
    set(INintervalSec);
  }
  
  return isOn();
}
