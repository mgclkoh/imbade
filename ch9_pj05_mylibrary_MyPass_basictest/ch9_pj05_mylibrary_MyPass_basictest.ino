#include <AES.h>
#include <AESLib.h>
#include <AES_config.h>
#include <xbase64.h>
#include "MyPass.h"
#include "MySerial.h"
MyPass myPass;
MySerial mySerial;

void setup() {
  Serial.begin(115200);
  Serial.println();

  Serial.println("Input > 'enc phrase' to encrypt a phrase");
  mySerial.begin(mySerialCallback); 
}

void loop() {
  mySerial.run();
}

void mySerialCallback(const String& key,const String& val) {
  if (key == "enc") {
    String v1 = val;
    String v2 = myPass.encHW(val);
    Serial.printf("Encrypted from [%s] to [%s]\n",
                  v1.c_str(),v2.c_str());
    String v3 = myPass.decHW(v2);
    Serial.printf("Decrypted from [%s] to [%s]\n",
                  v2.c_str(),v3.c_str());
  } 
}
