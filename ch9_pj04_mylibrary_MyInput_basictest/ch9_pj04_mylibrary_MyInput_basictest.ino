#include "MyInput.h"
#include "MyOutput.h"

#define BTN_PIN D3           // INPUT_PULLUP
#define LED_PIN LED_BUILTIN  // current sinking

MyInput btn(BTN_PIN,LOW,INPUT_PULLUP);
MyOutput led(LED_PIN,LOW);

void setup() {
  Serial.begin(115200);
  Serial.println();
}

/*
//==============================
//===btn.pressed() test
//버튼을 누를 때마다 led toggling-ok! 
//==============================
void loop() {
  led.run();
  btn.run();
  if (btn.pressed()) {
    Serial.println("Button press");
    led.toggle();
    if(digitalRead(LED_PIN)){
      Serial.println("LED: off");
    }
    else{
      Serial.println("LED: on");
    }    
  }
}
*/
/*
//==============================
//===btn.count() test
//버튼을 1.5초 안에 여러번 누른 후 1.5초 동안 누르지 않으면 
//누른 횟수를 저장한 후 저장된 횟수만큼 led on/off-ok! 
//==============================
void loop() {
  led.run();
  btn.run();
  int clickCnt = btn.count(); 
  if (clickCnt > 0) {
    Serial.printf("click count: %d\n", clickCnt);
    Serial.printf("LED is %d blinking...!\n", clickCnt);
    led.set(500,300,clickCnt);
  }
}
*/

/*
//==============================
//===btn.released()/btn.longReleased() test
//버튼을 짧게 눌렀다 떼면 1초간 led가 on되고
//길게(1.5초 이상) 눌렀다 떼면 led를 4회 점멸-ok!
//==============================
void loop() {
  led.run();
  btn.run();
  if (btn.released()) {
    Serial.println("Button short(->) released");
    led.on(1000);
  }  
  else if (btn.longReleased()) {
    Serial.println("Button long(----->) released");
    led.set(300,200,4);
  }
}
*/
///*
//==============================
//===btn.released()/btn.onLongPressedFirst() test
//버튼을 짧게 눌렀다 떼면 1초간 led가 on되고
//길게(1.5초 이상) 누르고 있으면 led를 4회 점멸-ok!
//==============================
void loop() {
  led.run();
  btn.run();
  if (btn.released()) {
    Serial.println("Button short(->) released");
    led.on(1000);
  }  
  else if (btn.onLongPressedFirst()) {
    Serial.println("Button long(========>) press");
    led.set(300,200,4);
  }
}
//*/
