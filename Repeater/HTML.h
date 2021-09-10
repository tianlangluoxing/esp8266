#ifndef __HTML_H__
#define __HTML_H__


const char index_html[] PROGMEM = {"<!DOCTYPE html><html><head><meta charset='utf-8'><meta name='viewport'content='width=device-width, initial-scale=1,maximum-scale=1,user-scalable=no'><title>桥接配置</title><style type='text/css'>html{height:100%;width:100%}body{}*{box-sizing:border-box}.content{position:absolute;left:50%;top:50%;width:430px;height:550px;margin:-300px 0 0-215px;border:1px solid#333;border-radius:20px;overflow:hidden;padding:50px 30px 50px 30px;font-family:cursive}.title{width:100%;height:48px;text-align:center;font-size:26px;margin-bottom:0px}.lable{width:100%;height:48px;border:1px solid#C2C2C2;line-height:48px;margin-bottom:10px;border-radius:24px;padding:0px 24px;font-size:18px;position:relative}.label label{color:#333333}.lable input{height:45px;width:calc(100%-48px);outline:none;border:none;padding-left:85px;position:absolute;top:1px;left:24px;background-color:rgba(255,255,255,0)}.line{width:100%;height:15px}.submit{width:100%;height:48px;border:1px solid#333;border-radius:20px;line-height:48px;text-align:center;margin-top:50px;cursor:pointer}.submit:active{position:relative;top:2px;background-color:#FFFFFF}@media screen and(max-width:600px){.content{position:static;margin:0;width:100%;height:100vh;border:none;padding:100px 10px 0 10px}.title{margin-bottom:30px}.submit{margin-top:60px}}</style></head><body><div class='content'><div class='title'>ESP8266 WiFi桥接</div><div class='lable'><label>wifi账号:</label><input type='text'id='sta_ssid'maxlength='31'value=''placeholder='请输入你要拓展的WiFi名称'/></div><div class='lable sta_end'><label>wifi密码:</label><input type='text'id='sta_pass'value=''maxlength='31'placeholder='请输入你要拓展的WiFi密码'/></div><div class='line'></div><div class='lable'><label>热点名称:</label><input type='text'id='ap_ssid'value=''maxlength='31'placeholder='请输入你要设置的WiFi名称'/></div><div class='lable'><label>热点密码:</label><input type='text'id='ap_pass'value=''maxlength='31'placeholder='请输入你要设置的WiFi密码'/></div><div class='lable'><label>MAC地址:</label><input type='text'id='mac'value=''maxlength='17'placeholder='请输入你要设置的MAC地址'/></div><div class='submit'id='submit'>确定</div></div><script type='text/javascript'>function checkMAC(MAC){var reg_name=/[0-9A-Fa-f\d]{2}:[0-9A-Fa-f\d]{2}:[0-9A-Fa-f\d]{2}:[0-9A-Fa-f\d]{2}:[0-9A-Fa-f\d]{2}:[0-9A-Fa-f\d]{2}/;if(!reg_name.test(MAC)){alert('mac地址格式不正确！mac地址格式为00:24:21:19:BD:E4');return false;}return true;}function post_html(sendData){var xmlhttp;if(window.XMLHttpRequest){xmlhttp=new XMLHttpRequest()}else{xmlhttp=new ActiveXObject('Microsoft.XMLHTTP')};xmlhttp.onreadystatechange=function(){if(xmlhttp.readyState==4&&xmlhttp.status==200){console.log(xmlhttp.responseText);let data=JSON.parse(xmlhttp.responseText);if(data.data.status==200){console.log('成功');alert('连接中，请稍后，当网络连接成功后灯保持常亮！')}else{alert('WiFi名称不存在或WiFi密码错误！')}}};xmlhttp.open('POST',`/config?ssid=${sendData.sta_ssid}&pass=${sendData.sta_pass}&ap_ssid=${sendData.ap_ssid}&ap_pass=${sendData.ap_pass}&MAC=${sendData.MAC}`,true);xmlhttp.send()}var submit=document.getElementById('submit');submit.onclick=function(){var sendData={sta_ssid:document.getElementById('sta_ssid').value,sta_pass:document.getElementById('sta_pass').value,ap_ssid:document.getElementById('ap_ssid').value,ap_pass:document.getElementById('ap_pass').value,MAC:document.getElementById('mac').value};if(sendData.sta_ssid.length<1){alert('请输入你要拓展的WiFi名称');return}if(sendData.sta_pass.length<8){alert('密码长度至少8位');return}if(sendData.ap_ssid.length<1){alert('请输入你要设置的WiFi名称');return}if(sendData.ap_pass.length<8){alert('密码长度至少8位');return}if(sendData.MAC){if(checkMAC(sendData.MAC)){post_html(sendData)}}else{post_html(sendData)}}</script></body></html>"};















#endif //__HTML_H__
