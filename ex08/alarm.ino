#include <WiFi.h>
#include <WebServer.h>

const int LED_PIN   = 2;
const int TOUCH_PIN = 4;
const char* ap_ssid = "ESP32-ALARM";
const char* ap_pwd  = "12345678";
const int TOUCH_THRESH = 60;  // 👈 提高灵敏度！更容易触发

WebServer server(80);
bool isArmed = false;
bool isAlarming = false;

void alarmFlash()
{
  digitalWrite(LED_PIN, HIGH);  // 报警亮
  delay(80);
  digitalWrite(LED_PIN, LOW);   // 报警灭
  delay(80);
}

String getHtmlPage()
{
  String stateText;
  if(isAlarming)
    stateText = "<span style='color:red;font-size:28px;font-weight:bold'>⚠️ 警报触发</span>";
  else if(isArmed)
    stateText = "<span style='color:orange;font-size:28px;font-weight:bold'>已布防警戒</span>";
  else
    stateText = "<span style='color:green;font-size:28px;font-weight:bold'>已撤防安全</span>";

  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1.0">
<style>
body{text-align:center;margin-top:80px;font-family:微软雅黑;background:#f5f5f5;}
h1{font-size:36px;color:#222;}
.btn{font-size:22px;padding:16px 40px;margin:25px 15px;border:none;border-radius:8px;cursor:pointer;}
.arm{background:#ff5555;color:white;}
.disarm{background:#22aa55;color:white;}
</style>
</head>
<body>
<h1>物联网安防报警系统</h1>
<p>)rawliteral" + stateText + R"rawliteral(</p>
<br>
<a href="/arm"><button class="btn arm">布防警戒</button></a>
<a href="/disarm"><button class="btn disarm">解除布防</button></a>
</body>
</html>
)rawliteral";
  return html;
}

void handleRoot()
{
  server.send(200,"text/html;charset=utf-8",getHtmlPage());
}

void handleArm()
{
  isArmed = true;
  isAlarming = false;
  digitalWrite(LED_PIN, LOW);  // 👈 布防 → 灯灭
  server.sendHeader("Location","/");
  server.send(303);
}

void handleDisarm()
{
  isArmed = false;
  isAlarming = false;
  digitalWrite(LED_PIN, LOW);  // 👈 撤防 → 灯彻底熄灭
  server.sendHeader("Location","/");
  server.send(303);
}

void setup()
{
  pinMode(LED_PIN,OUTPUT);
  digitalWrite(LED_PIN, LOW);  // 👈 开机 → 灯灭
  Serial.begin(115200);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid,ap_pwd);

  server.on("/",handleRoot);
  server.on("/arm",handleArm);
  server.on("/disarm",handleDisarm);
  server.begin();
}

void loop()
{
  server.handleClient();

  if(isAlarming)
  {
    alarmFlash();
    return;
  }

  if(isArmed)
  {
    int touchVal = touchRead(TOUCH_PIN);
    if(touchVal < TOUCH_THRESH)
    {
      isAlarming = true;
    }
  }
}