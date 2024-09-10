#define greenPin 13
#define yellowPin 10
#define buttonPin 7
#define pinkPin 5

void setup() {
  pinMode(greenPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(pinkPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
    digitalWrite(greenPin, HIGH);
    if (!digitalRead(buttonPin)) {
        delay(2000);
        digitalWrite(greenPin, LOW);
        digitalWrite(yellowPin, HIGH);
        delay(2000); 
        digitalWrite(yellowPin, LOW);
        digitalWrite(pinkPin, HIGH);
        delay(7000); 
        digitalWrite(pinkPin, LOW);
        return;
    }
    delay(50);
}