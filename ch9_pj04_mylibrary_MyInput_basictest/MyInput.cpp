#include <Arduino.h>
#include "MyInput.h"

/*****
base for constructor
*****/

void MyInput::init(int INbtnPin, int INcloseVal, int INpinMode) {

  btnPin = INbtnPin;
  pinMode(btnPin,INpinMode);
    
  btnCloseVal = INcloseVal;

  btnOpenVal = !btnCloseVal;

  pinVal = btnOpenVal;
  PRpinVal = btnOpenVal;
}

/*****
constructor
::에 주목하자 
no return type
*****/

MyInput::MyInput(int INbtnPin, int INcloseVal, int INpinMode) {

  init(INbtnPin, INcloseVal, INpinMode);
}

MyInput::MyInput(int INbtnPin) {

  init(INbtnPin, LOW, INPUT_PULLUP);
}
MyInput::MyInput() {

  init(255, LOW, INPUT_PULLUP);
}

void MyInput::setPin(int INbtnPin, int INcloseVal, int INpinMode) {

  init(INbtnPin, INcloseVal, INpinMode);
}

void MyInput::setPin(int INbtnPin) {

  init(INbtnPin, LOW, INPUT_PULLUP);
}

/*****
run repeatedly in loop()
*****/  

int  MyInput::run() {
    // 디바운스이면 
  if (debounceStarted && millis() - debounceStartMil <= DEBOUNCE_MIL) {
    pinStatus = RUN_STATUS_ON_DEBOUNCE;
    return pinStatus;
  } 

  debounceStarted = false;

  pinVal = digitalRead(btnPin);
  
  if(btnCloseVal == HIGH)  {
    pinVal = !pinVal;
  }

  // 디바운스 시작 때와 다른 상태 이면 
  if (PRpinStatusDebounce != PRpinStatus || pinStatusDebounce !=  pinStatus) {
    // 이전 핀값과 달라지면 디바운스 시작 
    if (PRpinVal != pinVal) { 
         debounceStarted = true;
       debounceStartMil = millis();
       PRpinStatusDebounce = PRpinStatus;
       pinStatusDebounce =  pinStatus;
       pinStatus = RUN_STATUS_ON_DEBOUNCE;
       return pinStatus;
       //delay(DEBOUNCE_MIL);
    } 
  } 
  
  PRpinStatus = pinStatus;
  pinStatus = PRpinVal << 1 | pinVal; 

  PRpinVal = pinVal; 

    // reset count when released
  //if (pinStatus == RUN_STATUS_RELEASED) {
  if (pinVal == HIGH) {  // released
      onLongClick = false;
    onLongPressedCnt = 0;
  }
  
  // start: pressCnt
  thisMil = millis();   
  if (pinStatus == RUN_STATUS_PRESSED) {  
    if (thisMil > pressedMil + CNT_DELAY_MIL ) {
      TMPpressedCnt = 0;
    }
    TMPpressedCnt++;
    pressedMil = thisMil;
  }
  // end: pressCnt
  #if SAN_INPUT_DELAY_USE
  delay(SAN_INPUT_DELAY_MIL);
  #endif 
    
  return pinStatus; 
}

/*****
pressed()
*****/

bool MyInput::pressed() {
    
  if (pinStatus == RUN_STATUS_PRESSED) {
    RTNstatus = STATUS_PRESSED;
    return true;
  }
  else 
    return false;

}

/*****
released()
*****/

bool MyInput::released() {
  
  if (pinStatus == RUN_STATUS_RELEASED && 
    (!useShortLongReleased || (useShortLongReleased && thisMil <= pressedMil + LONG_DELAY_MIL))) {
    if (SWvoidReleased) {
      SWvoidReleased = false;
      return false;
    }
    else {
      RTNstatus = STATUS_RELEASED;
      return true;
    }
  }
  else {
    //SWvoidReleased = false;
    return false;
  }

}

bool MyInput::shortClick() {
    return released();
}

/*****
longReleased()
*****/

bool MyInput::longReleased() {
  
  useShortLongReleased = true;
  
  if (pinStatus == RUN_STATUS_RELEASED && thisMil > pressedMil + longDelayMil) {
    RTNstatus = STATUS_LONG_RELEASED;
    return true;
  }
  else {
    return false;
  }

}

/*****
onPressed()
*****/

bool MyInput::onPressed() {
  
  if (pinStatus == RUN_STATUS_ON_PRESSED && 
    (!useShortLongPressed || (useShortLongPressed && thisMil <= pressedMil + LONG_DELAY_MIL))) {
    RTNstatus = STATUS_ON_RELEASED;
    return true;
  }   
  else {
    return false;
  }
    
}

bool MyInput::onPressedFirst() {
  if (PRpinStatus != pinStatus && onPressed()) {
    RTNstatus = STATUS_ON_PRESSED_FIRST;
    return true;
  }
  else 
    return false; 
}

/*****
onLongPressed()
*****/

bool MyInput::onLongPressed() {
  
  useShortLongPressed = true;
  
  if (pinStatus == RUN_STATUS_ON_PRESSED && thisMil > pressedMil + longDelayMil) {
    //int pinStatus; 
    onLongPressedCnt++;
    PRpinStatus = STATUS_ON_LONG_PRESSED;     
    pressedMil = millis();
    SWvoidReleased = true; 
    return true;
  }
  else {
    return false;
  }
    
}

bool MyInput::onLongPressedFirst() {

  useShortLongPressed = true;
  
  if (pinStatus == RUN_STATUS_ON_PRESSED && thisMil > pressedMil + LONG_DELAY_MIL) {
    //int pinStatus; 
    onLongPressedCnt++;
    //if (onLongPressedCnt == 1) {
    if (onLongPressedCnt == 1 && !onLongClick) {
      PRpinStatus = STATUS_ON_LONG_PRESSED_FIRST;     
      pressedMil = millis();
      SWvoidReleased = true; 
      TMPpressedCnt = 0;
      onLongClick = true;
      return true;
    }
    else {
      return false;
    } 

  }
  else {
    return false;
  }  
}

bool MyInput::longClick() {
  return onLongPressedFirst();  
}


bool MyInput::longPressed() {
  return onLongPressedFirst();  
}

/*****
onReleased()
*****/

bool MyInput::onReleased() {
  
  if (pinStatus == RUN_STATUS_ON_RELEASED) {
    RTNstatus = STATUS_ON_RELEASED;
    return true;
  }
  else 
    return false;
    
}

bool MyInput::onReleasedFirst() {

  if (PRpinStatus != pinStatus && onReleased()) {
    RTNstatus = STATUS_RELEASED_FIRST;
    return true;
  }
  else 
    return false; 
}

/*****
click(int noOfClick)
*****/

boolean MyInput::click(int INclick) {
    
  //if (thisMil > pressedMil + CNT_DELAY_MIL) {
  if (thisMil > pressedMil + CNT_DELAY_MIL) {
    pressedCnt = TMPpressedCnt;
  }

  //if (INclick == pressedCnt) {
    if (INclick == pressedCnt && pinVal == HIGH) {
    TMPpressedCnt = 0;
    pressedCnt = 0;   
    RTNstatus = STATUS_CLICK;
    return  true;
  }
  else 
    return false;
}

/*****
click()
*****/

boolean MyInput::click() {   
  return click(1);
}

/*****
click(int noOfClick)
*****/

int MyInput::count() {
  
  if (thisMil > pressedMil + CNT_DELAY_MIL) {   
    int cnt = TMPpressedCnt;

    TMPpressedCnt = 0;
    pressedCnt = 0;   
    RTNstatus = STATUS_COUNT;

    return cnt;
  }
  else {
    return 0;
  }
}

/*****
voidReleased
*****/

void MyInput::avoidReleased() {
  SWvoidReleased = true;
}

/*****
setLongDelayMil
*****/

void MyInput::setLongDelayMil(unsigned long INlongDelayMil) {
  longDelayMil = INlongDelayMil;
}

/*****
status
*****/

int MyInput::status() {  
  return pinStatus;
}

/*****
read
*****/

int MyInput::read() {  
  return pinVal;
}
