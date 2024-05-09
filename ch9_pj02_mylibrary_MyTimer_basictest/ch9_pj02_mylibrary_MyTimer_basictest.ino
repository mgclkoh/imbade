#include "MyTimer.h"

#define LED_PIN LED_BUILTIN
#define LED_ON  LOW  // sink mode
#define LED_OFF HIGH  // sink mode

double timer1IntervalSec = 2.5;
double timer2IntervalSec = 5.0;

MyTimer timer1(timer1IntervalSec); //ForLedToggle
MyTimer timer2; //ForPrint

bool ledIsOn = false;

void setup() {  
  Serial.begin(115200);
  Serial.println();

  pinMode(LED_PIN,OUTPUT);
  digitalWrite(LED_PIN,LED_OFF);
 
}

void loop() {
  if (timer1.isOn()) {
    //code for every timer1IntervalSec
    Serial.printf("timer1.isOn every %lf\n", timer1IntervalSec);
    ledToggle();
  }
  else if (timer2.isOn(timer2IntervalSec)) {
    //code for every timer2IntervalSec 
    //Serial.println("timerPrt.isOn()");   
    Serial.printf("-----timer2.isOn(%lf)\n", timer2IntervalSec);
  }
}


void ledToggle() {
  ledIsOn = !ledIsOn;
  if(ledIsOn) {      
    digitalWrite(LED_PIN,LED_ON);
  }
  else {  
    digitalWrite(LED_PIN,LED_OFF);
  }
  Serial.printf("led=%d\n",ledIsOn);
}
