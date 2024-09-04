// Engine ports
#define analogicEnableA 11
#define analogicEnableB 10
#define in1 9
#define in2 8
#define in3 7
#define in4 6
#define engineFront 70
#define engineBack 120

// Series Reflector
#define IN_D0 13
#define IN_D1 12

void engine_start(bool in_1, bool in_2, bool in_3, bool in_4, int delayEngine, int engineSpeedA, int engineSpeedB) {
  if (in_1 || in_2) analogWrite(analogicEnableA, engineSpeedA);
  if (in_3 || in_4) analogWrite(analogicEnableB, engineSpeedB);

  if (in_1) digitalWrite(in1, HIGH);
  if (in_2) digitalWrite(in2, HIGH);
  if (in_3) digitalWrite(in3, HIGH);
  if (in_4) digitalWrite(in4, HIGH);

  delay(delayEngine);

  if (in_1) digitalWrite(in1, LOW);
  if (in_2) digitalWrite(in2, LOW);
  if (in_3) digitalWrite(in3, LOW);
  if (in_4) digitalWrite(in4, LOW);

  if (in_1 || in_3) analogWrite(analogicEnableA, LOW);
  if (in_2 || in_4) analogWrite(analogicEnableB, LOW);
}

void engine_front() {
  engine_start(HIGH, LOW, HIGH, LOW, 50, engineFront, engineFront);  // 60
}

void engine_back() {
  engine_start(LOW, HIGH, LOW, HIGH, 100, engineBack, engineBack);
}

void engine_right() {
  engine_start(HIGH, LOW, LOW, HIGH, 320, engineBack, engineFront);
}

void engine_left() {
  engine_start(LOW, HIGH, HIGH, LOW, 320, engineFront, engineBack);
}

void setup() {
  // Serial configuration
  Serial.begin(9600);

  // Engine configuration
  pinMode(analogicEnableA, OUTPUT);
  pinMode(analogicEnableB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // Reflector
  pinMode(IN_D0, INPUT);
  pinMode(IN_D1, INPUT);
}

void loop() {
  int reflection_right = digitalRead(IN_D0);
  int reflection_left = digitalRead(IN_D1);

  Serial.print("Right = ");
  Serial.println(reflection_right);
  Serial.print("Left = ");
  Serial.println(reflection_left);

  if (reflection_right) {
    engine_left();
    return;
  }

  if (reflection_left) {
    engine_right();
    return;
  }

  engine_front();
}