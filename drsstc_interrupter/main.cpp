#include <LiquidCrystal.h>
#include "encoder.h"

// compile-time maxiumums, for safety
const long maxOnTime = 120;	// on time in uS
const long maxBPS = 300;	// max BPS

// inputs
const int modePin = A0; // Mode select
			
const int Enc1PinA = A2; // BPS (fixed, burst)
const int Enc1PinB = A3;
const int Enc2PinA = A4; // Burst length (burst, oneshot)
const int Enc2PinB = A5;
const int Enc3PinA = 9;  // Burst rate (burst, oneshot)
const int Enc3PinB = 10;

// outputs
const int outputPin = LED_BUILTIN;
const int lcdRS = 12;
const int lcdEN = 11;
const int lcdD4 = 5;
const int lcdD5 = 4;
const int lcdD6 = 3;
const int lcdD7 = 2;

int mode;
int BPS;
unsigned long onTime, PRF, nextOnTime, nextOffTime;
char txt[15];

// initialize LCD library
LiquidCrystal lcd(lcdRS, lcdEN, lcdD4, lcdD5, lcdD6, lcdD7);

void doFixed();
void doBurst();
void doMIDI();
void resetEncoders();
void readEncoders();

typedef void (* ModeFP)();
ModeFP Modes[3] = {&doFixed, &doBurst, &doMIDI};
Encoder encoders[3] = { 
	{Enc1PinA, Enc1PinB},
	{Enc2PinA, Enc2PinB},
	{Enc3PinA, Enc3PinB}
};

void setup() {
	pinMode(modePin, INPUT);
	pinMode(outputPin, OUTPUT);

	lcd.begin(16, 2);

	// starting values
	mode = 0;
	BPS = 0;
	onTime = 0;
	PRF = 0;
	nextOnTime = 0;
	nextOffTime = 0;
	lcd.print("Booting...");
	delay(2000);
	resetEncoders();
}

int getNextMode(int m) {
	int nextMode = m++;
	if (nextMode > int(sizeof(Modes)) - 1) {
		return 0;
	} else {
		return nextMode;
	}
}

// fixed mode
void doFixed() {
	lcd.setCursor(0, 0);
	lcd.print("Fixed");

	BPS = encoders[0].getPosition();
	onTime = encoders[1].getPosition();
	
	lcd.setCursor(6, 0);
	sprintf(txt, "BPS: %i", BPS);
	lcd.print(txt);

	lcd.setCursor(7, 1);
	sprintf(txt, "ON: %lus", onTime);
	lcd.print(txt);

	return;
}

// burst mode
void doBurst() {
	lcd.setCursor(0, 0);
	lcd.print("Burst");
	lcd.setCursor(0, 1);
	lcd.print("Not implemented");
	return;
}

// MIDI mode
void doMIDI() {
	lcd.setCursor(0, 0);
	lcd.print("MIDI");
	lcd.setCursor(0, 1);
	lcd.print("Not implemented");
	return;
}

// loop through and reset all encoders to 0
void resetEncoders() {
	for (unsigned int i; i < sizeof(encoders)/sizeof(encoders[0]); i++) {
		encoders[i].setPosition(0);
	}
}

// loop through and read values on all encoders
void readEncoders() {
	for (unsigned int i; i < sizeof(encoders)/sizeof(encoders[0]); i++) {
		encoders[i].readPosition();
	}
}

void loop() {
	// check our inputs
	if (digitalRead(modePin) == HIGH) {
		// change mode
		mode = getNextMode(mode);
		// reset encoders to 0
		resetEncoders();
	}
	// read values from all encoders
	readEncoders();

	// run chosen mode function
	Modes[mode]();
}


