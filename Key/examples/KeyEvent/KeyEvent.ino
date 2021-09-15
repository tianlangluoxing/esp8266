/**
 * 按键事件库
 * 
 * 作者：落星  
 * 
 * 此按键事件库可应用与ArduinoIED 编写的程序中，不受开发板类型限制
 * 此按键事件库提供按键上拉，下拉两种模式，在使用下拉模式时，务必在电路中加上下拉电阻，以免影响程序运行效果
 * 此事件库提供5个事件，即按下事件、松开事件、按下不放事件、按下超时事件、双击事件
 * 此按键事件库采用轮询的方式实现，受程序中的延时延时影响较大，建议使用在微延时的程序中
 * 
 * 延时的影响
 * 当使用双击事件时，如果在两次按下的期间，如果程序中存在大量延时，总延时超过1S，则无法触发双击事件
 * 当使用按下超时事件时，注意，此事件触发事件总比设定的触发事件要长，并受程序轮询一次总时长的影响
 * 
 * 
 * 此按键事件库开放使用，但不提供任何承诺与保证
 * 作者保留此程序的所有权
 * 
 */
#include <Key.h>

/**
 * 实例化按键
 * 并设定引脚0为按键输入引脚
 * 默认按键内部sahngla
 */
KEY k(0);

/*
 * 实例化按键
 * 并设定引脚0为按键输入引脚
 * 设置上拉模式：PULLUP
 * 设置下拉模式：PULLDOWN，建议使用下拉模式时外接下拉电阻
 */
//KEY k(0, PULLUP);

/*
 * 按键按下触发事件
 */
void onKeyDown() {
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  Serial.printf("onKeyDown\n");
}

/*
 * 按键松开触发事件
 */
void onKeyUp() {
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  Serial.printf("onKeyUp\n");
}

/*
 * 按键按下不放触发事件
 */
void onkeypress() {
  Serial.printf("onkeypress\n");
}

/*
 * 按键按下超时触发事件
 */
void onkeypressTimeOut() {
  Serial.printf("onkeypressTimeOut\n");
}

/*
 * 按键双击触发事件
 */
void onDoubleClick() {
  Serial.printf("onDoubleClick\n");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  /*
   * 注册按键按下事件
   */
  k.setKeydownCallback(onKeyDown);

   /*
   * 注册按键松开事件
   */
  k.setKeyupCallback(onKeyUp);

   /*
   * 注册按键按下不放事件
   */
//  k.setKeypressCallback(onkeypress);

 /*
   * 注册按键按下超时事件
   * 其中3000为触发事件，单位ms
   * 触发时间请根据需要修改
   * 注意，触发时间比设定的触发时间要长
   */
  k.setKeypressTimeOutCallback(onkeypressTimeOut, 3000);

   /*
   * 注册按键双击事件
   * 双击时间间隔为1S以内有效
   * 两次按下期间不能有较长时间的delay()，否则双击事件可能无法触发
   */
  k.setDoubleClick_Callback(onDoubleClick);
}

void loop() {
  // put your main code here, to run repeatedly:
  /*
   * 按键事件轮询
   */
  k.loop();
}
