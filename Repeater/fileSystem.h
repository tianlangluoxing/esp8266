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

void initE2prom() {
  EEPROM.begin(EEPROM_LEN);
}

bool isConfig() {
  if (EEPROM.read(CONFIG_FLAG) == 1) {
    return true;
  } else {
    return false;
  }
}

void config() {
  EEPROM.write(CONFIG_FLAG, 1);
}

void writeData(uint8_t addr, const char* data, uint8_t len) {
  EEPROM.write(addr, len);
  Serial.println("start write");
  for (int index = 1; index <= len; index ++) {
    EEPROM.write(addr + index, data[index - 1]);
    Serial.print(addr + index);
  }
  EEPROM.commit();
  Serial.println("\nend write");
}

void readData(uint8_t addr, char* data) {
  uint8_t len = EEPROM.read(addr);

  //  Serial.printf("read start %d, length:%d\n", addr, EEPROM.read(addr));
  //  Serial.printf("read start %d, length:%d\n", addr, len);
  if (len > 30 || len == 0) {
    data = {0};
    return;
  }
  for (int index = 1; index <= len; index ++) {
    data[index - 1] = EEPROM.read(addr + index);
  }
  data[len] = '\0';
  //  Serial.printf("read start %d, Data:%s\n", addr, data);
}


void save_STA_SSID(const char* ssid) {
  writeData(SSID_STA_ADDR, ssid, strlen(ssid));
}
uint8_t read_STA_SSID(char *ssid) {
  readData(SSID_STA_ADDR, ssid);
  Serial.printf("read_STA_SSID: %s\n", ssid);
  return strlen(ssid);
}

void save_STA_PASS(const char* pass) {
  writeData(PASS_STA_ADDR, pass, strlen(pass));
}
uint8_t read_STA_PASS(char *pass) {
  readData(PASS_STA_ADDR, pass);
  Serial.printf("read_STA_PASS: %s\n", pass);
  return strlen(pass);
}

void save_AP_SSID(const char* ssid) {
  writeData(SSID_AP_ADDR, ssid, strlen(ssid));
}
uint8_t read_AP_SSID(char *ssid) {
  readData(SSID_AP_ADDR, ssid);
  Serial.printf("read_AP_SSID: %s\n", ssid);
  return strlen(ssid);
}

void save_AP_PASS(const char* pass) {
  writeData(PASS_AP_ADDR, pass, strlen(pass));
}
uint8_t read_AP_PASS(char *pass) {
  readData(PASS_AP_ADDR, pass);
  Serial.printf("read_AP_PASS: %s\n", pass);
  return strlen(pass);
}

void save_MAC(const char* MAC) {
  writeData(MAC_ADDR, MAC, strlen(MAC));
}
uint8_t read_MAC(char *MAC) {
  readData(MAC_ADDR, MAC);
  Serial.printf("read_MAC: %x\n", MAC);
  return strlen(MAC);
}

void resetSystem() {
  for (int index = 0; index < EEPROM_LEN; index++) {
    EEPROM.write(index, 0);
  }
}

















#endif //__FILE_SYSTEM_H__
