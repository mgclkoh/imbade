#ifndef MY_GET_CHIP_ID_H
#define MY_GET_CHIP_ID_H

uint32_t MyGetChipId() {
  uint32_t chipId = 0;

  #if defined(ESP8266)

  chipId = ESP.getChipId();

  #elif defined(ESP32) 

  uint64_t macAddress = ESP.getEfuseMac();
  uint64_t macAddressTrunc = macAddress << 40;
  chipId = macAddressTrunc >> 40;
  
  #endif

  return chipId;
}

#endif // #ifndef MY_GET_CHIP_ID_H
