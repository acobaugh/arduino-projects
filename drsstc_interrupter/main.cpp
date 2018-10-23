#include <Arduino.h>
#include <LiquidCrystal.h>
#include "main.h"
#include "encoder.h"
#include "timer.h"

// version string
static const char version[] = VERSION;

// compile-time maxiumums, for safety
const long maxOnTime = 120;	// on time in uS
const long maxBPS = 300;	// max BPS

const int countsPerDetent = 4;

// inputs
const int modePin = A0; // Mode select
			
const int Enc1PinA = A2; // BPS (fixed, burst)
const int Enc1PinB = A3;
const int Enc2PinA = A4; // Burst length (burst, oneshot)
const int Enc2PinB = A5;
const int Enc3PinA = 9;  // Burst rate (burst, oneshot)
const int Enc3PinB = 10;

// LCD
const int outputPin = 13; // LED pin
const int lcdRS = 12;
const int lcdEN = 11;
const int lcdD4 = 5;
const int lcdD5 = 4;
const int lcdD6 = 3;
const int lcdD7 = 2;

// output, defined as port/bit for speed
volatile uint8_t& outputPORTx PORTB;
volatile uint8_t& outputDDRx DDRB;
const int outputBit = 5;

// globals
int mode;
int BPS;
unsigned long onTime, PRF;
char txt[15];

ModeFP Modes[3] = {&doFixed, &doBurst, &doMIDI};
Encoder encoders[3] = { 
	{Enc1PinA, Enc1PinB},
	{Enc2PinA, Enc2PinB},
	{Enc3PinA, Enc3PinB}
};

// initialize LCD library
LiquidCrystal lcd(lcdRS, lcdEN, lcdD4, lcdD5, lcdD6, lcdD7);

void setup() {
	pinMode(modePin, INPUT_PULLUP);
	
	// set output port to output and low
	outputDDRx |= (1 << outputBit);
	outputPORTx &= ~(1 << outputBit);

	lcd.begin(16, 2);

	// starting values
	mode = 0;
	BPS = 0;
	onTime = 0;
	PRF = 0;

	sprintf(txt, "v%s", VERSION);
	lcd.setCursor(0, 0);
	lcd.print(txt);
	delay(1000);
	lcd.clear();
	resetEncoders();

	setupTimer();
}

int getNextMode(int m) {
	int nextMode = ++m;
	if (nextMode > int(sizeof(Modes)/sizeof(Modes[0])) - 1) {
		return 0;
	} else {
		return nextMode;
	}
}

// fixed mode
void doFixed() {
	lcd.setCursor(0, 0);
	lcd.print("Fixed");

	BPS = encoders[0].getPosition() / countsPerDetent;
	onTime = encoders[1].getPosition() / countsPerDetent;

	if (BPS < 0) BPS = 0;
	if (onTime < 0) onTime = 0;

	lcd.setCursor(6, 0);
	sprintf(txt, "BPS: %03i", BPS);
	lcd.print(txt);

	lcd.setCursor(7, 1);
	sprintf(txt, "ON: %03ius", int(onTime));
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
	for (unsigned int i = 0; i < sizeof(encoders)/sizeof(encoders[0]); i++) {
		encoders[i].setPosition(0);
	}
}

// loop through and read values on all encoders
void readEncoders() {
	for (unsigned int i = 0; i < sizeof(encoders)/sizeof(encoders[0]); i++) {
		encoders[i].readPosition();
	}
}

void loop() {
	// check our inputs
	if (digitalRead(modePin) == LOW) {
		// change mode
		mode = getNextMode(mode);
		// reset encoders to 0
		resetEncoders();
		lcd.clear();
		lcd.print("....");
		delay(300);
		lcd.clear();
	}
	// read values from all encoders
	readEncoders();

	// run chosen mode function
	Modes[mode]();
}


