#define buttonPin 7
#define ledPin 12

int is_activated = false;

void setup()
{
  // Configure pins
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin,OUTPUT);
}

void loop()
{
  // Verify user input
  int pressed = digitalRead(buttonPin);
  
  // Change LED status
  if (pressed == LOW)
    is_activated = !is_activated;
  
  // Verify LED
  if (is_activated)
    digitalWrite(ledPin,HIGH);
  else
    digitalWrite(ledPin,LOW);
  delay(50);
}
