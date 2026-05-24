// 多档位触摸调速呼吸灯（3档速度 差距极大）
#define TOUCH_PIN 4    // 触摸引脚 D4
#define LED_PIN   2    // LED引脚
#define THRESHOLD 30   // 触摸阈值

volatile bool toggle = false;

// 呼吸灯变量
int brightness = 0;
int fadeStep = 5;
int speedMode = 1;  // 1慢 2中 3快

// 【关键】延时差距拉到最大！
int delayTime[] = {30, 10, 2};  // 超慢 → 中速 → 超快

void touchISR() {
  toggle = true;
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  touchAttachInterrupt(TOUCH_PIN, touchISR, THRESHOLD);
  Serial.println("3档触摸调速呼吸灯(差距极大版)");
}

void nextSpeed() {
  speedMode++;
  if (speedMode > 3) speedMode = 1;

  Serial.print("当前档位：");
  Serial.println(speedMode);
}

void loop() {
  // 触摸切换档位
  if (toggle) {
    delay(80);
    if (touchRead(TOUCH_PIN) < THRESHOLD) {
      nextSpeed();
    }
    toggle = false;
  }

  // 呼吸灯逻辑
  brightness += fadeStep;
  if (brightness <= 0 || brightness >= 255) {
    fadeStep = -fadeStep;
  }

  // 板载LED反向点亮
  analogWrite(LED_PIN, 255 - brightness);

  // 档位延时（差距巨大）
  delay(delayTime[speedMode - 1]);
}