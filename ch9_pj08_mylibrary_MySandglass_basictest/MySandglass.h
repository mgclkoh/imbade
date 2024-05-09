#ifndef MY_SANDGLASS_H
#define MY_SANDGLASS_H

#ifndef SANDGLASS_SEC
#define SANDGLASS_SEC 10
#endif  //#ifndef SANDGLASS_SEC


class MySandglass {

  public:
    
    MySandglass(float durationSec);
    MySandglass();
    void MySandglassBase(float durationSec);

    void on();
    void on(float durationSec);
    void off();
    void toggle();

    void setDuration(float durationSec);
    void setDuration();

    bool toOn();  
    bool toOff();

    int status();
    String status(const String& onStr,const String& offStr);

    bool isOn(); 
    bool isOff();  

    float getDuration();      
    float getRemain();      
 

  private:

    bool _active;
    bool _to_on_token;
    bool _to_off_token;

    float durationSec;
    long durationMil;
    unsigned long durationUntilMil;

};

#endif
