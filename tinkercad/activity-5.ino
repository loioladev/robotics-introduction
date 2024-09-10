#define yellowLedPin 5
#define blueLedPin 4
#define greyLedPin 3
#define orangeLedPin 2
#define greenLedPin 1
#define redLedPin 0
#define yellowButtonPin 13
#define blueButtonPin 12
#define greyButtonPin 11
#define orangeButtonPin 10

// Define game configuration
const int rounds = 5;
const int colorsQuantity = 4;

// Define global variables
int colors[colorsQuantity] = {yellowLedPin, blueLedPin, greyLedPin, orangeLedPin};
int orderColors[rounds];
int user[rounds];
int roundIndex = 0;
int endgame = 0;

void setup() {
	// Pin configuration
	pinMode(yellowLedPin, OUTPUT);
	pinMode(blueLedPin, OUTPUT);
	pinMode(greyLedPin, OUTPUT);
	pinMode(orangeLedPin, OUTPUT);
	pinMode(greenLedPin, OUTPUT);
	pinMode(redLedPin, OUTPUT);
	pinMode(yellowButtonPin, INPUT_PULLUP);
	pinMode(blueButtonPin, INPUT_PULLUP);
	pinMode(greyButtonPin, INPUT_PULLUP);
	pinMode(orangeButtonPin, INPUT_PULLUP);

	// Define random colors
	randomSeed(analogRead(0));
	for (int i = 0; i < rounds; i++)
		orderColors[i] = random(0, colorsQuantity);
}

bool compareColors(int index) {
    /* Compare colors to see if user was right */
    for (int i = 0; i <= index; i++)
        if (orderColors[i] != user[i]) return false;  
    return true;  
}

void showColors() {
	/* Show the colors in the led */
	for (int i = 0; i <= roundIndex; i++) {
		int colorHigh = colors[orderColors[i]];
		digitalWrite(colorHigh, HIGH);
		delay(500);
		digitalWrite(colorHigh, LOW);
		delay(250);
	}
}

int askUserForColor() {
	/* Make user choose the color */
	delay(50);
	for (int i = 0; i < colorsQuantity; i++)
		digitalWrite(colors[i], HIGH);

	// Make loop to wait user input
	int colorChosen = -1;
	while(true) {
		if (!digitalRead(yellowButtonPin)) 
			colorChosen = 0;
		else if (!digitalRead(blueButtonPin)) 
			colorChosen = 1;
		else if (!digitalRead(greyButtonPin)) 
			colorChosen = 2;
		else if (!digitalRead(orangeButtonPin))
			colorChosen = 3;
		else 
			continue;
		break;
	}

	for (int i = 0; i < colorsQuantity; i++)
		digitalWrite(colors[i], LOW);
	delay(50);
	return colorChosen;
}


void loop() {
	// Verify if the game has ended
	if (endgame == 1) {
		digitalWrite(greenLedPin, HIGH);
		return;
	}
	if (endgame == 2) {
		digitalWrite(redLedPin, HIGH);
		return;
	}

	for (int r = 0; r < rounds; r++) {
		showColors();
		for (int i = 0; i <= roundIndex; i++) {
			user[i] = askUserForColor();

			// Verify answer given by the user
			bool correctAnswer = compareColors(i);
			if (!correctAnswer) {
				endgame = 2;
				return;
			}
		}
		roundIndex++;
	}
	endgame = 1;
}