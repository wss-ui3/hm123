#include <WiFi.h>
#include <WebServer.h>

const int LED_PIN = 2;
const char* ap_ssid = "ESP32-LAB";
const char* ap_pass = "12345678";

WebServer server(80);
int currentBrightness = 0;

// 修复：数字越大 → 灯越亮！
void setLED(int value) {
  value = constrain(value, 0, 255);
  currentBrightness = value;
  analogWrite(LED_PIN, value);  // 这里改回正常！
}

String makePage() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>实时调光</title>
</head>
<body style="text-align:center; margin-top:50px;">
  <h1>LED实时亮度调节</h1>
  <p>当前亮度：<span id="val">0</span></p>
  
  <input type="range" id="slider" min="0" max="255" value="0" style="width:300px;">
  
  <script>
    let slider = document.getElementById("slider");
    let valText = document.getElementById("val");
    
    slider.oninput = function() {
      valText.innerText = this.value;
      fetch("/pwm?pwm=" + this.value);
    };
  </script>

  <br><br>
  <button onclick="fetch('/on'); slider.value=255; valText.innerText=255;" style="padding:10px 20px;">全亮</button>
  <button onclick="fetch('/off'); slider.value=0; valText.innerText=0;" style="padding:10px 20px;">全灭</button>
</body>
</html>
)rawliteral";
  return html;
}

void handleRoot() {
  server.send(200, "text/html; charset=utf-8", makePage());
}

void handlePWM() {
  if (server.hasArg("pwm")) {
    int val = server.arg("pwm").toInt();
    setLED(val);
  }
  server.send(200, "text/plain", "OK");
}

void handleOn() {
  setLED(255);
  server.send(200, "text/plain", "ON");
}

void handleOff() {
  setLED(0);
  server.send(200, "text/plain", "OFF");
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  setLED(0);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_pass);

  server.on("/", handleRoot);
  server.on("/pwm", handlePWM);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  
  server.begin();
}

void loop() {
  server.handleClient();
}