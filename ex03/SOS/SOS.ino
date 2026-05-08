#define ledPin 2

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  //=====================
  // S：短闪 3 次
  //=====================
  for(int i=0; i<3; i++) { 
    digitalWrite(ledPin, HIGH); 
    delay(200); 
    digitalWrite(ledPin, LOW); 
    delay(200); 
  }
  delay(500); // 字母间隔

  //=====================
  // O：长闪 3 次
  //=====================
  for(int i=0; i<3; i++) { 
    digitalWrite(ledPin, HIGH); 
    delay(600); 
    digitalWrite(ledPin, LOW); 
    delay(200); 
  }
  delay(500); // 字母间隔

  //=====================
  // S：短闪 3 次
  //=====================
  for(int i=0; i<3; i++) { 
    digitalWrite(ledPin, HIGH); 
    delay(200); 
    digitalWrite(ledPin, LOW); 
    delay(200); 
  }

  //=====================
  // ✅ 长停顿（必须有！）
  //=====================
  delay(3000); // 一轮 SOS 结束，停 3 秒再重新开始
}