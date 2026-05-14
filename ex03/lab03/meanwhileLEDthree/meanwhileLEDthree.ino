// 定义三个LED的引脚
const int redPin = 2;
const int yellowPin = 4;
const int greenPin = 5;

// PWM属性（和你原代码保持一致）
const int freq = 5000;
const int resolution = 8;

void setup() {
  Serial.begin(115200);
  
  // 为每个LED绑定PWM
  ledcAttach(redPin, freq, resolution);
  ledcAttach(yellowPin, freq, resolution);
  ledcAttach(greenPin, freq, resolution);
}

void loop() {
  // ========== 1. 红灯呼吸（先亮后灭） ==========
  Serial.println("红灯呼吸");
  for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){   
    ledcWrite(redPin, dutyCycle);
    delay(10);
  }
  for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){
    ledcWrite(redPin, dutyCycle);
    delay(10);
  }

  // ========== 2. 黄灯呼吸（先亮后灭） ==========
  Serial.println("黄灯呼吸");
  for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){   
    ledcWrite(yellowPin, dutyCycle);
    delay(10);
  }
  for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){
    ledcWrite(yellowPin, dutyCycle);
    delay(10);
  }

  // ========== 3. 绿灯呼吸（先亮后灭） ==========
  Serial.println("绿灯呼吸");
  for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){   
    ledcWrite(greenPin, dutyCycle);
    delay(10);
  }
  for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){
    ledcWrite(greenPin, dutyCycle);
    delay(10);
  }

  Serial.println("一轮顺序呼吸完成");
}