/*
 * NAPT协议下的路由桥接
 * 
 * 这是一个最简单的版本
 * 打开STA模式,将esp8266连接到路由器
 * 再打开AP模式,生成一个AP热点
 * 再通过NAPT协议将AP热点的网络地址转接到STA网络上
 * 
 * 具体情况,请自行了解NAPT协议
 * 
 * 程序来源,esp8266 arduino例程的 RangeEatender-NAPT
 * 现只是进行了封装及添加了MAC地址修改部分(主要用于钉钉打卡)
 * 
 * 板砖者: 落星
 * 时间: 2021-8-26
 */

#define HAVE_NETDUMP 1

#ifndef STASSID
#define STASSID "luoxing_2.4"
#define STAPSK  "ABCluoxing123"
#define APSSID "sxdmn1.com"
#define APPSK  "z12345678"
uint8_t newMACAddress[] = {0x10, 0xDA, 0x43, 0x70, 0x9A, 0xDD};
#endif

#include <ESP8266WiFi.h>
#include <lwip/napt.h>
#include <lwip/dns.h>
#include <dhcpserver.h>
#include "fileSystem.h"

#define NAPT 1000
#define NAPT_PORT 10

/*
 * 配置你要连接的WiFi
 */
void setSTA() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }
  Serial.printf("\nSTA: %s (dns: %s / %s)\n",
                WiFi.localIP().toString().c_str(),
                WiFi.dnsIP(0).toString().c_str(),
                WiFi.dnsIP(1).toString().c_str());
}

/*
 * 配置你要开启的AP热点
 */
void setAP() {
  WiFi.softAPConfig(  // enable AP, with android-compatible google domain
    IPAddress(172, 217, 28, 254),
    IPAddress(172, 217, 28, 254),
    IPAddress(255, 255, 255, 0));
  WiFi.softAP(APSSID, APPSK);
  Serial.printf("AP: %s\n", WiFi.softAPIP().toString().c_str());
}
/*
 * 修改MAC地址
 */
void setMAC(){
  wifi_set_macaddr(SOFTAP_IF, &newMACAddress[0]);
  delay(200);
  uint8_t MACAddress[6];
  if(wifi_get_macaddr(SOFTAP_IF,MACAddress)){
    Serial.print("MAC: ");
    for(int i=0;i<6;i++){
      Serial.print(MACAddress[i],HEX);
      if(i<5){
        Serial.print(":");
      }
    }
    Serial.println();
  }
}
/*
 * 开启网络地址转换
 */
void setNAPT(){
  err_t ret = ip_napt_init(NAPT, NAPT_PORT);
  Serial.printf("ip_napt_init(%d,%d): ret=%d (OK=%d)\n", NAPT, NAPT_PORT, (int)ret, (int)ERR_OK);
  if (ret == ERR_OK) {
    ret = ip_napt_enable_no(SOFTAP_IF, 1);
    Serial.printf("ip_napt_enable_no(SOFTAP_IF): ret=%d (OK=%d)\n", (int)ret, (int)ERR_OK);
    if (ret == ERR_OK) {
      Serial.printf("WiFi Network '%s' with the password is  '%s'\n", APSSID, APPSK);
    }
  }
  if (ret != ERR_OK) {
    Serial.printf("NAPT initialization failed\n");
  }
}

void setup() {
  
  EEPROM.begin(512);
  Serial.begin(115200);
  Serial.printf("\n\nNAPT Range extender\n");
  Serial.printf("Heap on start: %d\n", ESP.getFreeHeap());

  // first, connect to STA so we can get a proper local DNS server
  setSTA();


  // give DNS servers to AP side
  dhcps_set_dns(0, WiFi.dnsIP(0));
  dhcps_set_dns(1, WiFi.dnsIP(1));

  setAP();
  setMAC();
  
  Serial.printf("Heap before: %d\n", ESP.getFreeHeap());
  setNAPT();
  
  Serial.printf("Heap after napt init: %d\n", ESP.getFreeHeap());
 
}

void loop() {
}
