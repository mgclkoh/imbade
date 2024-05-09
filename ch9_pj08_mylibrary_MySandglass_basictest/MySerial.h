#ifndef SAN_SERIAL_H
#define SAN_SERIAL_H

#ifndef SERIAL_DELAY_MIL
#define SERIAL_DELAY_MIL 2
#endif

#ifndef SAN_SERIAL_STR_LEN

#if defined(ESP8266) || defined(ESP32)
#define SAN_SERIAL_STR_LEN 255
#else
#define SAN_SERIAL_STR_LEN 63
#endif

#endif

class MySerial {

  public:
    
  // &Serial can be omitted. 
  // First argument can be  &Serial1,&SoftSerial etc.
  
  MySerial ();
  MySerial (void(*callback)(const String& key,const String& val));
  MySerial (Stream* myStream,void(*callback)(const String& key,const String& val));

  MySerial (void(*callback)(String key,String val));
  MySerial (Stream* myStream,void(*callback)(String key,String val));
  
  void begin();
  void begin(void(*callback)(const String& key,const String& val));
  void begin(Stream* myStream,void(*callback)(const String& key,const String& val));

  void begin(void(*callback)(String key,String val));
  void begin(Stream* myStream,void(*callback)(String key,String val));


    // This must be in loop()  
  void run();

  void echoOn();
  void echoOff();

  int  verifyBool(const String& payload);
  bool toBool(const String& payload);
   
  private: 
  void (*callback)(const String& key,const String& val);
  void (*callbackCopy)(String key,String val);

  Stream* myStream;

  // Wait for SERIAL_DELAY_MIL after one char received.
  int serialDelayMil = SERIAL_DELAY_MIL;

  bool isEchoOn = true;

  int cbType = 0; // call function type >> 0: ref(const String&), 1:copy(String)
    
};
// ↑ ; must be after class.

#endif
