#ifndef __FILE_SYSTEM_H__
#define __FILE_SYSTEM_H__

#include <EEPROM.h>

#define CONFIG_FLAG     0
#define SSID_STA_ADDR   32
#define PASS_STA_ADDR   64
#define SSID_AP_ADDR    96
#define PASS_AP_ADDR    128
#define MAC_ADDR        160
#define EEPROM_LEN      256

void initE2prom(){
  EEPROM.begin(EEPROM_LEN);
}

bool isConfig(){
  if(EEPROM.read(CONFIG_FLAG) == 1){
    return true;
  }else{
    return false;
  }
}

void writeData(uint8_t addr, const char* data, uint8_t len) {
  EEPROM.write(addr, len);
  for (int index = 1; index <= len; index ++) {
    EEPROM.write(addr + index, *data ++);
  }
}

void readData(uint8_t addr, const char* data){
  uint8_t len = EEPROM.read(addr);
  for (int index = 1; index <= len; index ++) {
    *data++ = EEPROM.read(addr + index);
  }
}

void resetSystem(){
  for(int index = 0; index < EEPROM_LEN; index++){
    EEPROM.write(index, 0);
  }
}

















#endif //__FILE_SYSTEM_H__
