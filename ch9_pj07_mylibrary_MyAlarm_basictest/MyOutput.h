#ifndef SAN_OUTPUT_H
#define SAN_OUTPUT_H
#include <Arduino.h>

#ifndef SAN_OUTPUT_DELAY_MIL
#define SAN_OUTPUT_DELAY_MIL 3
#endif

#define STAGE_IDLE  0 
#define STAGE_INIT  1 
#define STAGE_ON  2 
#define STAGE_OFF 3 
#define STAGE_CYCLE_DELAY 4

#define CMND_IDLE -1
#define CMND_OFF 0
#define CMND_ON 1    

#define STATUS_BLINK 2
#define STATUS_OFF 0
#define STATUS_ON 1

class MyOutput {

  public:
    MyOutput(int pinNo,int onVal); 
    MyOutput(int pinNo);           // onVal = HIGH
    MyOutput(); 

    void setPin(int pinNo,int onVal); 
    void setPin(int pinNo);           // onVal = HIGH   
    
    // put in outputISR rather than loop()
    void run();

    void on();
    void off();
    void toggle();

    void on(unsigned long onMil);
    void off(unsigned long offMil);


    void set(unsigned long onMil,unsigned long offMil,int onOffCnt,int onOffcycle,unsigned long onOffCycleDelayMil);
      void set(unsigned long onMil,unsigned long offMil,int onOffCnt,int onOffcycle);
    void set(unsigned long onMil,unsigned long offMil,int onOffCnt);
    void set(unsigned long onMil,unsigned long offMil);

    void set(int onOffSatus);
    void set(const String& onOffSatus);
    void set(const char* onOffSatus);   

    bool isOn();      // pinStatus: 1||-1
    bool isOff();     // pinStatus: 0 
    bool isBlink();   // pinStatus: -1  

    int status();     // 1 (pinStatus: 1 || -1), 0 (pinStatus: 0)
    int toBool(const String& payload);  // 1,0
    int status3();  // pinStatus (0,1,-1)
    int val();        // actual pin value(0,1)

    String status(const String& onStr, const String& offStr);
    String status3(const String& onStr, const String& offStr,const String& blinkStr);
    
    
    void setEndOn();  // on after blink
    void setEndOff(); // off after blink
  
  private:
    void init(int pinNo,int onVal);   
    void setBase(unsigned long onMil,unsigned long offMil,int onOffCnt,int onOffcycle,unsigned long onOffCycleDelayMil);
  
    int pinNo;
    int onVal;  
    int offVal;
  
    volatile int onMil;
    volatile int offMil;
    volatile int onOffCnt;
    volatile int onOffCycle;
    volatile int onOffCycleDelayMil;
    
    volatile int RunOnOffCnt;
    volatile int RunOnOffCycle;

    volatile int onOffCntDown;
    volatile int onOffCycleDown;
    
    volatile boolean onOffCntForever = false;
    volatile boolean onOffCycleForever = false;

    volatile boolean setEnd = false;

    

    volatile int runStage = STAGE_IDLE; // 0: idle, 1: init, 2: on, 3:of, 4: cycle delay

    volatile unsigned long thisMil;
    volatile unsigned long nextMil;

    volatile int pinCmnd = CMND_OFF;
    
    volatile boolean runModifyOn = 0;

    volatile int pinStatus = STATUS_OFF; 
    volatile int SVpinStatus = STATUS_OFF; 

};

#endif
