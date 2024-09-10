#define greenPin 3
#define redPin 4
#define passwordSize 4
#define num1Pin 13
#define num2Pin 12
#define num3Pin 11
#define num4Pin 10
#define num5Pin 9
#define num6Pin 8
#define num7Pin 7

// CAFE password
const int correctPassword[passwordSize] = {2, 6, 4, 3};

int password[passwordSize];
int digits = 0;

void setup() {
    /* Configure pins of board */
    pinMode(greenPin, OUTPUT);
    pinMode(redPin, OUTPUT);
    pinMode(num1Pin, INPUT_PULLUP);
    pinMode(num2Pin, INPUT_PULLUP);
    pinMode(num3Pin, INPUT_PULLUP);
    pinMode(num4Pin, INPUT_PULLUP);
    pinMode(num5Pin, INPUT_PULLUP);
    pinMode(num6Pin, INPUT_PULLUP);
    pinMode(num7Pin, INPUT_PULLUP);
}


bool comparePasswords() {
    /* Compare passwords to see if user was right */
    for (int i = 0; i < passwordSize; i++)
        if (password[i] != correctPassword[i]) return false;  
    return true;  
}


void loop() { 
    // Verify if 4 digits were pressed
    if (digits == passwordSize) {
        if (comparePasswords()) digitalWrite(greenPin, HIGH);
        else digitalWrite(redPin, HIGH);
        return;
    }

    // Read input
    if (!digitalRead(num1Pin)) 
        password[digits] = 1;
    else if (!digitalRead(num2Pin)) 
        password[digits] = 2;
    else if (!digitalRead(num3Pin)) 
        password[digits] = 3;
    else if (!digitalRead(num4Pin)) 
        password[digits] = 4;
    else if (!digitalRead(num5Pin)) 
        password[digits] = 5;
    else if (!digitalRead(num6Pin)) 
        password[digits] = 6;
    else if (!digitalRead(num7Pin)) 
        password[digits] = 7;
    else 
        return;

    // Update password
    digits++;
    delay(50);
}