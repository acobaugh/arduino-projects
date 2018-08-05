#include <LiquidCrystal.h>

// compile-time maxiumums, for safety
const long maxOnTime = 300;	// on time in uS
const long maxBPS = 300;	// max BPS

// inputs
const int modePin = A0;
const int playStopPin = A1;

// outputs
const int outputPin = LED_BUILTIN;
const int lcdRS = 12;
const int lcdEN = 11;
const int lcdD4 = 5;
const int lcdD5 = 4;
const int lcdD6 = 3;
const int lcdD7 = 2;

const int minMode = 1;
const int maxMode = 2;

int m;
int BPS;
unsigned long onTime, PRF, nextOnTime, nextOffTime;

// initialize LCD library
LiquidCrystal lcd(lcdRS, lcdEN, lcdD4, lcdD5, lcdD6, lcdD7);

void setup() {
	pinMode(modePin, INPUT);
	pinMode(playStopPin, INPUT);
	pinMode(outputPin, OUTPUT);

	lcd.begin(16, 2);

	// starting values
	m = minMode;
	BPS = 1;
	onTime = 0;
	PRF = BPS;
	nextOnTime = 0;
	nextOffTime = 0;
}

void loop() {
  // check our inputs
	if (digitalRead(modePin) == HIGH) {
		m = getNextMode(m);
	}

	switch(m) {
    		default:
		case 1:
			doManual;
			break;
		case 2:
			doMIDI;
			break;
	}
}

int getNextMode(int m) {
	int nextMode = m++;
	if (nextMode > maxMode) {
		return minMode;
	} else {
		return nextMode;
	}
}

void doManual() {
	// time to turn on
	if (micros() > nextOnTime) {
		digitalWrite(outputPin, HIGH);
		nextOffTime = nextOnTime + onTime;
	}
	
	// time to turn off
	if (micros() > nextOffTime) {
		digitalWrite(outputPin, LOW);
		// current offTime + number of uS per bang
		nextOnTime = nextOffTime + (1 / BPS * 1000000);
	}
	return;
}

void doMIDI() {}
