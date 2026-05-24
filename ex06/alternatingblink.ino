const int LEDA = 2;
const int LEDB = 4;
int val = 0;
int step = 1;

void setup()
{
  pinMode(LEDA, OUTPUT);
  pinMode(LEDB, OUTPUT);
}

void loop()
{
  // A亮度递增，B亮度递减，严格反相
  analogWrite(LEDA, 255 - val);
  analogWrite(LEDB, val);

  val += step;
  // 触界反转方向
  if(val >= 255 || val <= 0)
  {
    step = -step;
  }
  delay(8);
}