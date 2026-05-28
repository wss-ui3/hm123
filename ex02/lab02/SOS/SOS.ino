const int LED_PIN = 2;

// 时间参数(ms)，拉大差值，视觉区分明显
const unsigned long SHORT_ON  = 150;   // 短闪点亮时间
const unsigned long LONG_ON   = 800;   // 长闪点亮时间
const unsigned long GAP       = 200;   // 单个闪烁之间间隔
const unsigned long SOS_PAUSE = 2000;   // 一组SOS结束后长停顿

// 运行状态
unsigned long currentTime;
unsigned long previousTime = 0;
int state = 0;  // 0:空闲 1:三短闪 2:三长闪 3:末尾三短闪
int blinkCnt = 0;
bool ledStatus = false;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  currentTime = millis();

  switch (state) {
    case 0:
      // 空闲等待，延时结束开始新一轮SOS
      if (currentTime - previousTime >= SOS_PAUSE) {
        state = 1;
        blinkCnt = 0;
        ledStatus = true;
        digitalWrite(LED_PIN, HIGH);
        previousTime = currentTime;
      }
      break;

    // 第一组：3次短闪
    case 1:
      if (ledStatus) {
        // 点亮时长结束，熄灭
        if (currentTime - previousTime >= SHORT_ON) {
          ledStatus = false;
          digitalWrite(LED_PIN, LOW);
          previousTime = currentTime;
        }
      } else {
        // 间隔结束，准备下一次闪烁
        if (currentTime - previousTime >= GAP) {
          blinkCnt++;
          if (blinkCnt >= 3) {
            // 3次短闪完成，进入长闪阶段
            state = 2;
            blinkCnt = 0;
          }
          ledStatus = true;
          digitalWrite(LED_PIN, HIGH);
          previousTime = currentTime;
        }
      }
      break;

    // 第二组：3次长闪
    case 2:
      if (ledStatus) {
        if (currentTime - previousTime >= LONG_ON) {
          ledStatus = false;
          digitalWrite(LED_PIN, LOW);
          previousTime = currentTime;
        }
      } else {
        if (currentTime - previousTime >= GAP) {
          blinkCnt++;
          if (blinkCnt >= 3) {
            // 3次长闪完成，进入最后一组短闪
            state = 3;
            blinkCnt = 0;
          }
          ledStatus = true;
          digitalWrite(LED_PIN, HIGH);
          previousTime = currentTime;
        }
      }
      break;

    // 第三组：末尾3次短闪
    case 3:
      if (ledStatus) {
        if (currentTime - previousTime >= SHORT_ON) {
          ledStatus = false;
          digitalWrite(LED_PIN, LOW);
          previousTime = currentTime;
        }
      } else {
        if (currentTime - previousTime >= GAP) {
          blinkCnt++;
          if (blinkCnt >= 3) {
            // 整组SOS完成，回到空闲状态
            state = 0;
          }
          ledStatus = true;
          digitalWrite(LED_PIN, HIGH);
          previousTime = currentTime;
        }
      }
      break;
  }
}