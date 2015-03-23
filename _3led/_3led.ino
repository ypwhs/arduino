int a = 9, b = 10, c = 11;

void setup() {
  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
}

void loop() {
  int i;
  for (i = 0; i < 256; i = i + 5)
  {
    analogWrite(a, i);
    delay(5);
  }
  for (i = 0; i < 256; i = i + 5)v
  {
    analogWrite(b, i);
    delay(5);
  }
  for (i = 255; i > 0; i = i - 5)
  {
    analogWrite(a, i);
    delay(5);
  }
  for (i = 0; i < 256; i = i + 5)
  {
    analogWrite(c, i);
    delay(5);
  }
  for (i = 255; i > 0; i = i - 5)
  {
    analogWrite(b, i);
    delay(5);
  }
  for (i = 0; i < 256; i = i + 5)
  {
    analogWrite(a, i);
    delay(5);
  }
  for (i = 255; i > 0; i = i - 5)
  {
    analogWrite(c, i);
    delay(5);
  }
  for (i = 255; i > 0; i = i - 5)
  {
    analogWrite(a, i);
    delay(5);
  }
}

