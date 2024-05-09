#include <Arduino.h>
#include "MySandglass.h"

void MySandglass::MySandglassBase(float INdurationSec) {
    _active         = false;
    _to_on_token    = false;
    _to_off_token   = false;

    durationSec     = INdurationSec;
    durationMil     = durationSec * 1000;
    durationUntilMil = 0;
}

// digital pin
MySandglass::MySandglass(float INdurationSec) {
  MySandglassBase(INdurationSec); 
}

MySandglass::MySandglass() {
  MySandglassBase(SANDGLASS_SEC);
}

void MySandglass::on() {
    durationUntilMil = millis() + durationMil;

    _active         = true;
    _to_on_token    = true;
    _to_off_token   = true;
}

void MySandglass::on(float INdurationSec) {
  durationSec = INdurationSec;
  durationMil = durationSec * 1000;
  on();
}

void MySandglass::off() {
    durationUntilMil = 0;

    _active         = false;
    _to_on_token    = false;
    _to_off_token   = true;
}

void MySandglass::toggle() {

  if (_active) {
    off();
  }
  else {
    on();
  }
}

void MySandglass::setDuration(float INdurationSec) {
  durationSec = INdurationSec;
  durationMil = durationSec * 1000;
}

void MySandglass::setDuration() {
  setDuration(SANDGLASS_SEC);
}


bool MySandglass::toOn() {
    if (_to_on_token) {
        _to_on_token = false;
        return true;
    }
    else {
        return false;
    }
}

bool MySandglass::toOff() {
    if (_active) {
        if (millis() > durationUntilMil) {
            _active = false;
        }
    }
  if (_to_off_token && !_active) {
        _to_off_token = false;
        return true;
    }
    else {
        return false;
    }
}

bool MySandglass::isOn() {
    if (_active) {
        if (millis() > durationUntilMil) {
            _active = false;
        }
    }
    return _active;
}

bool MySandglass::isOff() {
    if (_active) {
        if (millis() > durationUntilMil) {
            _active = false;
        }
    }
    return !_active;
}

int MySandglass::status() {
    if (_active) {
        if (millis() > durationUntilMil) {
            _active = false;
        }
    }
  return _active;
}

String MySandglass::status(const String& onStr,const String& offStr) {
    if (_active) {
        if (millis() > durationUntilMil) {
            _active = false;
        }
    }

    if (_active) {
        return onStr;
    }
    else {
        return offStr;
    }
}

float MySandglass::getDuration() {
    return durationSec;
}

float MySandglass::getRemain() {

    if (_active) { 
        return (durationUntilMil - millis()) / 1000.0;
    }
    else {
        return 0;
    }
}
