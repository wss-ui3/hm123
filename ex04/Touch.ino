#define TOUCH_PIN 4    // 触摸引脚 D4
#define LED_PIN   2    // LED引脚
#define THRESHOLD 500 // 你板子专用阈值

volatile bool toggle = false;  // 中断触发标记
bool ledState = false;         // LED状态

// 中断函数：只做标记，不操作LED
void touchISR() {
  toggle = true;
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // 初始灭

  // 开启触摸中断
  touchAttachInterrupt(TOUCH_PIN, touchISR, THRESHOLD);
  
  Serial.begin(115200);
  Serial.println("触摸开关就绪：摸一下 亮 / 再摸 灭");
}

void loop() {
  // 检测是否触发中断
  if (toggle) {
    delay(50); // 消抖
    
    // 确认真的摸到了，才切换一次
    if (touchRead(TOUCH_PIN) < THRESHOLD) {
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
      
      Serial.print("LED：");
      Serial.println(ledState ? "亮" : "灭");
    }
    
    toggle = false; // 清除标记
  }

  // 实时显示触摸值
  Serial.print("当前触摸值：");
  Serial.println(touchRead(TOUCH_PIN));
  delay(100);
}