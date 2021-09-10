/*
   NAPT协议下的路由桥接

   这是一个最简单的版本
   打开STA模式,将esp8266连接到路由器
   再打开AP模式,生成一个AP热点
   再通过NAPT协议将AP热点的网络地址转接到STA网络上

   具体情况,请自行了解NAPT协议

   程序来源,esp8266 arduino例程的 RangeEatender-NAPT
   现只是进行了封装及添加了MAC地址修改部分(主要用于钉钉打卡)

   板砖者: 落星
   时间: 2021-8-26
*/

#ifndef STASSID
#define STASSID "luoxing_2.4"
#define STAPASS  "ABCluoxing123"
#define APSSID "sxdmn1.com1"
#define APPASS  "z12345678"
uint8_t newMACAddress[] = {0x10, 0xDA, 0x43, 0x70, 0x9A, 0xDD};
#endif

#include <ESP8266WiFi.h>
#include <lwip/napt.h>
#include <lwip/dns.h>
#include <LwipDhcpServer.h>
#include <ESP8266WebServer.h>
#include "fileSystem.h"
#include "HTML.h"

#define NAPT 1000
#define NAPT_PORT 10

ESP8266WebServer server(80);

/*
   配置你要连接的WiFi
*/
void setSTA(const char *ssid, const char *pass) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
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
   配置你要开启的AP热点
*/
void setAP(const char *ssid, const char *pass) {
  WiFi.softAPConfig(  // enable AP, with android-compatible google domain
    IPAddress(172, 217, 28, 254),
    IPAddress(172, 217, 28, 254),
    IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid, pass);
  Serial.printf("AP: %s\n", WiFi.softAPIP().toString().c_str());
}

/*
   小写转大写
*/
void lowerCase2uppercase(char *DEST, const char *SRC) {
  int len = strlen(SRC);
  for (int index = 0; index < len; index++) {
    if (SRC[index] >= 'a' &&  SRC[index] <= 'z') {
      DEST[index] = char( SRC[index] - ('a' - 'A'));
    }
    else {
      DEST[index] = SRC[index];
    }
  }
  DEST[len] = '\0';
}

/*
   String类型的MAC地址转uint8_t类型的MAC地址
*/
void stringMAC2MAC(uint8_t *MAC, String S_MAC) {
  char CS_MAC[20] = {0}; //= const_cast<char*>(S_MAC.c_str());
  lowerCase2uppercase(CS_MAC, S_MAC.c_str()); //转为大写
  //  Serial.println(CS_MAC);
  char C_MAC[12] = {0};
  char *result = NULL;
  result = strtok(CS_MAC, ":");
  while (result != NULL) {
    strcat(C_MAC, result);
    result = strtok(NULL, ":");
  }
  for (int index = 0; index < 6; index ++) {
    uint8_t s1 = C_MAC[index * 2] - '0';
    if (s1 > 9) {
      s1 = s1 - 7;
    }
    uint8_t s2 = C_MAC[index * 2 + 1] - '0';
    if (s2 > 9) {
      s2 = s2 - 7;
    }
    MAC[index] = s1 * 16 + s2;
  }
}

/*
   修改MAC地址
*/
void setMAC(uint8_t *MAC) {
  wifi_set_macaddr(SOFTAP_IF, &MAC[0]);
  //  wifi_set_macaddr(SOFTAP_IF, &newMACAddress[0]);
  delay(200);
  uint8_t MACAddress[6];
  if (wifi_get_macaddr(SOFTAP_IF, MACAddress)) {
    Serial.print("MAC: ");
    for (int i = 0; i < 6; i++) {
      Serial.print(MACAddress[i], HEX);
      if (i < 5) {
        Serial.print(":");
      }
    }
    Serial.println();
  }
}
/*
   开启网络地址转换
*/
void setNAPT(const char *ssid, const char *pass) {
  err_t ret = ip_napt_init(NAPT, NAPT_PORT);
  Serial.printf("ip_napt_init(%d,%d): ret=%d (OK=%d)\n", NAPT, NAPT_PORT, (int)ret, (int)ERR_OK);
  if (ret == ERR_OK) {
    ret = ip_napt_enable_no(SOFTAP_IF, 1);
    Serial.printf("ip_napt_enable_no(SOFTAP_IF): ret=%d (OK=%d)\n", (int)ret, (int)ERR_OK);
    if (ret == ERR_OK) {
      Serial.printf("WiFi Network '%s' with the password is  '%s'\n", ssid, pass);
    }
  }
  if (ret != ERR_OK) {
    Serial.printf("NAPT initialization failed\n");
  }
}

void setup() {

  initE2prom();
  Serial.begin(115200);
  Serial.println();

  Serial.printf("\n\nNAPT Range extender\n");
  Serial.printf("Heap on start: %d\n", ESP.getFreeHeap());

  /**
     连接WiFi
     如果有配置WiFi
     则从配置中获取配置信息
  */
  char STA_SSIS[32] = {0};
  char STA_PASS[32] = {0};
  //  read_STA_SSID(STA_SSIS);
  if (read_STA_SSID(STA_SSIS) > 1 && read_STA_PASS(STA_PASS)) {
    setSTA(STA_SSIS, STA_PASS);
  }
  else {
    setSTA(STASSID, STAPASS);
  }


  // give DNS servers to AP side
  dhcpSoftAP.dhcps_set_dns(0, WiFi.dnsIP(0));
  dhcpSoftAP.dhcps_set_dns(1, WiFi.dnsIP(1));

  /**
       开启WiFi热点
       如果有配置WiFi热点
       则从配置中获取配置信息
  */
  char AP_SSID[32] = {0};
  char AP_PASS[32] = {0};
  bool AP_flag = false;
  if (read_AP_SSID(AP_SSID) > 1 && read_AP_PASS(AP_PASS)) {
    setAP(AP_SSID, AP_PASS);
    AP_flag = true;
  }
  else {
    setAP(APSSID, APPASS);
    AP_flag = false;
  }


  //  uint8_t _MAC[6] = {0};
  //  String S_MAC = "de:a0:4a:79:82:1c";
  //  stringMAC2MAC(_MAC, S_MAC);
  //  save_MAC((char*)_MAC);

  uint8_t MAC[6] = {0};
  if (read_MAC((char*)MAC) > 0) {
    setMAC((uint8_t*)MAC);
  }


  Serial.printf("Heap before: %d\n", ESP.getFreeHeap());
  if (AP_flag) {
    setNAPT(AP_SSID, AP_PASS);
  }
  else {
    setNAPT(APSSID, APPASS);
  }


  Serial.printf("Heap after napt init: %d\n", ESP.getFreeHeap());

  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", index_html);
  });

  server.on("/config", HTTP_POST, []() {
    if (server.hasArg("ssid") && server.hasArg("pass") && server.hasArg("ap_ssid") && server.hasArg("ap_pass")) {
      Serial.println(String(server.arg("ssid")));
      Serial.println(String(server.arg("pass")));
      Serial.println(String(server.arg("ap_ssid")));
      Serial.println(String(server.arg("ap_pass")));
      Serial.println(String(server.arg("MAC")));
      save_STA_SSID((String(server.arg("ssid"))).c_str());
      save_STA_PASS((String(server.arg("pass"))).c_str());
      save_AP_SSID((String(server.arg("ap_ssid"))).c_str());
      save_AP_PASS((String(server.arg("ap_pass"))).c_str());
      uint8_t _MAC[6] = {0};
      String S_MAC = server.arg("MAC");
      stringMAC2MAC(_MAC, S_MAC);
      save_MAC((char*)_MAC);
      //      save_MAC((String(server.arg("MAC"))).c_str());

      //      char STA_SSIS[32] = {0};
      //      char STA_PASS[32] = {0};
      //      char AP_SSID[32] = {0};
      //      char AP_PASS[32] = {0};
      //      uint8_t MAC[6] = {0};
      //      read_STA_SSID(STA_SSIS);
      //      read_STA_PASS(STA_PASS);
      //      read_AP_PASS(AP_PASS);
      //      read_AP_PASS(AP_PASS);
      //      read_MAC((char*)MAC);

      server.send(200, "application/json", "{\"data\":{\"status\":200}}");
      delay(1000);
      ESP.reset();
    }
    server.send(200, "application/json", "{\"data\":{\"status\":400}}");
  });
  server.onNotFound([]() {
    server.send(404, "text/plain", "NotFound");
  });
  server.begin();
}

void loop() {
  server.handleClient();
}
