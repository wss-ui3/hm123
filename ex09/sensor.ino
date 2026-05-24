#include <WiFi.h>
#include <WebServer.h>

const int TOUCH_PIN = 4;    // 触摸引脚
const char* ap_ssid = "ESP32-DASH";
const char* ap_pwd  = "12345678";

WebServer server(80);

// 主页（美观仪表盘）
void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>实时触摸传感器仪表盘</title>
  <style>
    body{
      text-align:center;
      margin-top:100px;
      font-family:Arial;
      background:#eef2f5;
    }
    .title{
      font-size:36px;
      color:#222;
      margin-bottom:40px;
    }
    .value{
      font-size:80px;
      font-weight:bold;
      color:#007bff;
      margin:40px 0;
    }
    .tips{
      font-size:24px;
      color:#555;
    }
  </style>
</head>
<body>
  <div class="title">ESP32 实时触摸传感器仪表盘</div>
  <div class="value" id="touchVal">0</div>
  <div class="tips">实时触摸值 | 手靠近变小，手离开变大</div>

  <script>
    // AJAX 实时刷新数值（无刷新）
    function updateValue() {
      fetch("/data").then(res=>res.text()).then(data=>{
        document.getElementById("touchVal").innerText = data;
      });
    }
    // 每 100ms 更新一次
    setInterval(updateValue, 100);
  </script>
</body>
</html>
  )rawliteral";
  
  server.send(200, "text/html; charset=utf-8", html);
}

// 返回实时触摸数据
void handleData() {
  int val = touchRead(TOUCH_PIN);
  server.send(200, "text/plain", String(val));
}

void setup() {
  Serial.begin(115200);
  
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_pwd);

  server.on("/", handleRoot);
  server.on("/data", handleData); // 实时数据接口
  server.begin();
}

void loop() {
  server.handleClient();
}