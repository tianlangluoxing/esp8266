/*
 * esp8266 钉钉WiFi打卡模拟器
 * 
 * 钉钉WiFi打卡原理
 * 打开钉钉,搜索附近WiFi,连接上之后匹配WiFi的MAC地址
 * 如果WiFi的MAC地址正确则自动打卡
 * 
 * 有的情况是搜索到WiFi并从WiFi广播中拿到正确的MAC地址后自动打卡
 * 
 * 注意
 * 因为此程序没有进行网络桥接,所以没有联网功能
 * 为了确保能够自动打卡,请先打开钉钉,再给板子上电
 * 
 * 注意
 * 为了百分百模拟,请修改你需要模拟的WiFi信息
 * ssid: 你需要模拟的打卡WiFi
 * password: 你需要模拟的WiFi密码
 * newMACAddress: 你需要模拟的WiFi的MAC地址
 * 
 * 关键
 * MAC地址一定是你要模拟的WiFi(路由器)的MAC地址
 * 
 * MAC地址一定是你要模拟的WiFi(路由器)的MAC地址
 * 
 * MAC地址一定是你要模拟的WiFi(路由器)的MAC地址
 * 
 * 此程序由 Arduino IDE 1.8.5编写,
 * esp8266 lib版本2.6.3
 * 作者: 落星
 * 时间: 2021-8-26
 */

#include <ESP8266WiFi.h>

const char *ssid = "sxdmn.com";//
const char *password = "z12345678";
uint8_t newMACAddress[] = {0x10, 0xDA, 0x43, 0x70, 0x9A, 0xDD};

void setup() {
  delay(1000);
  Serial.begin(115200);
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // For Soft Access Point (AP) Mode
  wifi_set_macaddr(SOFTAP_IF, &newMACAddress[0]);
  // For Station Mode
  //  wifi_set_macaddr(STATION_IF, &newMACAddress[0]);
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

void loop() {
}
