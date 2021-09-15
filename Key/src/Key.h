/*
   按键类
   按键默认接上拉

   作者：落星
*/
#ifndef __MY_KEY_H__
#define __MY_KEY_H__

#define PULLUP HIGH
#define PULLDOWN LOW


class KEY {
  private:
    int _pin = -1;//按键绑定引脚
    bool _pullModel = PULLUP;//按键上下拉模式
    bool _lastStatus = true;//按键上一状态，用于消抖
    bool _lastKeyStatus = true;//按键上一状态，用于事件
    unsigned long _lastkeyDownTime = 0;//按键按下时间，用于消抖
    unsigned long _lastkeyUpTime = 0;//按键谈起时间，用于消抖
    unsigned long _lastTimeOutTime = 0;//按键按下时间，用于按下超时事件
    unsigned long _timeOut = 0;//设置按下超时
    bool _timeOutFlag = false;//按下超时使能
    int _clickCount = 0;
    unsigned long _doubleClickTime = 0;
    void (*onkeydown_Callback)();//按下事件回调
    void (*onkeypress_Callback)();//按下不放事件回调
    void (*onkeyup_Callback)();//松开事件回调
    void (*onkeypressTimeOut_Callback)();//按下超时回调
    void (*onDoubleClick_Callback)();//按下超时回调

    void _doubleClick() {
      unsigned long nowTime = millis();
      _clickCount = (_clickCount + 1);
      if (_clickCount == 1) {
        _doubleClickTime = nowTime;
      }
      if (_clickCount == 2 && nowTime - _doubleClickTime < 1000 && onDoubleClick_Callback) {
        onDoubleClick_Callback();
        _clickCount = 0;
      }
      if (nowTime - _doubleClickTime > 1000) {
        _clickCount = 0;
      }
    }
  public:
    KEY(int pin) {
      _pin = pin;
      pinMode(_pin, INPUT_PULLUP);
      _lastStatus = digitalRead(_pin);
    }
    KEY(int pin, bool pullModel) {
      _pin = pin;
      _pullModel = pullModel;
      if (_pullModel == PULLUP) {
        pinMode(_pin, INPUT_PULLUP);
      }
      _lastStatus = digitalRead(_pin);
    }
    //设置按键按下回调
    KEY& setKeydownCallback(void(*callback)()) {
      Serial.printf("setKeydownCallback\n");
      this->onkeydown_Callback = callback;
      return *this;
    }
    //设置按键按下（一直按着）回调
    KEY& setKeypressCallback(void(*callback)()) {
      Serial.printf("setKeypressCallback\n");
      this->onkeypress_Callback = callback;
      return *this;
    }
    //设置按键按下（一直按着）超时回调
    KEY& setKeypressTimeOutCallback(void(*callback)(), unsigned long timeOut) {
      Serial.printf("setKeypressCallback\n");
      _timeOut = timeOut;
      this->onkeypressTimeOut_Callback = callback;
      return *this;
    }
    //设置按键弹起回调
    KEY& setKeyupCallback(void(*callback)()) {
      Serial.printf("setKeyupCallback\n");
      this->onkeyup_Callback = callback;
      return *this;
    }

    //设置按键弹起回调
    KEY& setDoubleClick_Callback(void(*callback)()) {
      Serial.printf("setKeyupCallback\n");
      this->onDoubleClick_Callback = callback;
      return *this;
    }

    //设置按键监听
    void loop() {
      bool keyStatus = digitalRead(_pin);
      unsigned long nowTime = millis();
      //获取按键稳定的最后一次状态
      if (keyStatus == LOW && _lastStatus == HIGH) {
        _lastkeyDownTime = nowTime;
      } else if (keyStatus == HIGH && _lastStatus == LOW) {
        _lastkeyUpTime = nowTime;
      }
      //按键由高电平转低电平稳定10毫秒之后执行事件
      if (keyStatus == LOW && _lastKeyStatus == HIGH && nowTime - _lastkeyDownTime > 10) { //消抖
        _lastKeyStatus = LOW;
        _lastTimeOutTime = nowTime;
        if (_pullModel && onkeydown_Callback) {
          onkeydown_Callback();
          _timeOutFlag = true;
        } else if (onkeyup_Callback) {
          onkeyup_Callback();
          _timeOutFlag = false;
        }
        if (_pullModel) {
          _doubleClick();
        }
      }
      //按键一直低电平
      if (_pullModel && keyStatus == LOW && _lastStatus == LOW && nowTime - _lastkeyUpTime > 10) {
        if (onkeypress_Callback) {
          onkeypress_Callback();
        }
        if (onkeypressTimeOut_Callback && (nowTime - _lastTimeOutTime > _timeOut) && _timeOutFlag) {
          onkeypressTimeOut_Callback();
          _timeOutFlag = false;
        }
      }
      //按键一直高电平
      if (!_pullModel && keyStatus == HIGH && _lastStatus == HIGH && nowTime - _lastkeyUpTime > 10) {
        if (onkeypress_Callback) {
          onkeypress_Callback();
        }
        if (onkeypressTimeOut_Callback && (nowTime - _lastTimeOutTime > _timeOut) && _timeOutFlag) {
          onkeypressTimeOut_Callback();
          _timeOutFlag = false;
        }
      }
      //按键由低电平转高电平稳定10毫秒之后执行事件
      if (keyStatus == HIGH && _lastKeyStatus == LOW && nowTime - _lastkeyUpTime > 10) { //消抖
        _lastKeyStatus = HIGH;
        _lastTimeOutTime = nowTime;
        if (_pullModel && onkeyup_Callback) {
          onkeyup_Callback();
          _timeOutFlag = false;
        } else if (onkeydown_Callback) {
          onkeydown_Callback();
          _timeOutFlag = true;
        }
        if (!_pullModel) {
          _doubleClick();
        }
      }
      _lastStatus = keyStatus;
      yield();
    }
};













#endif //__MY_KEY_H__
