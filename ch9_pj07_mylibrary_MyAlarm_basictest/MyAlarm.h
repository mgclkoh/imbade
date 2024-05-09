#ifndef MY_ALARM_H
#define MY_ALARM_H
#define ALARM_DIGITAL 0
#define ALARM_ANALOG 1

class MyAlarm {
  public:    
    MyAlarm(uint8_t pinNo, int activeVal, int inputMode, float alarmDurationSec);
    MyAlarm(uint8_t pinNo, int trigger, float alarmDurationSec);

    bool toOn();  
    bool toOff();

    bool isOn(); 
    bool isOff();

    void on();
    void off();

    int status();
    String status(const String& onStr,const String& offStr);

    void reset();

    void start();
    void stop();
    bool toStart();
    bool toStop();
    bool isStart();
    bool isStop();
    void toggleStartStop();

    int read();

    void setTrigger(int trigger);
    int getTrigger();
    
    void setDuration(float alarmDurationSec);
    float getDuration();

    float getRemain();

    void run();

    void setSyncLight(int tORf);
    bool isSyncLight();
    bool wasSyncLight();

    void setReportAlarm(int tORf);
    bool isReportAlarm();
    bool wasReportAlarm();

  private:
    bool toOnDigital();    
    bool toOffDigital();

    bool toOnAnalog();    
    bool toOffAnalog();

    int pinNo;
    int inputMode;
    int activeVal;
    long alarmDurationMil;
    float alarmDurationSec;

    int triggerVal;

    int pinRead;
    int SVpinRead;

    int alarmOn;
    int startOn;
    int toOnManual;
    int toOffManual;
    int digitalOrAnalog; // 0:digital, 1:analog

    bool isToStart;
    bool isToStop;
    unsigned long alarmOnUntilMil;

    bool _isSyncLight = false;
    bool _wasSyncLight = false;
    bool _isReportAlarm = true;
    bool _wasReportAlarm = true;
};

#endif
