#include "encoder.h"
#include "main.h"

Encoder::Encoder(int8_t pinA, int8_t pinB) {	
	_pinA = pinA;
	_pinB = pinB;

	_position = 0;

	curModeA = LOW;
	curModeB = LOW;
	lastModeA = curModeA;
	lastModeB = curModeB;
}

void Encoder::readPosition() {
	// read the current state of the encoder's pins
	curModeA = digitalRead(_pinA);
	curModeB = digitalRead(_pinB);

	if (curModeA != lastModeA) {
		if (curModeA == LOW) {
			if (curModeB == LOW) {
				_position--;
			} else {
				_position++;
			}
		} else {
			if (curModeB == LOW) {
				_position++;
			} else {
				_position--;
			}
		}
	}

	if (curModeB != lastModeB) {
		if (curModeB == LOW) {
			if (curModeA == LOW) {
				_position++;
			} else {
				_position--;
			}
		} else {
			if (curModeA == LOW) {
				_position--;
			} else {
				_position++;
			}
		}
	}

	lastModeA = curModeA;
	lastModeB = curModeB;
}

void Encoder::setPosition(long int position) {
	_position = position;
}

long int Encoder::getPosition() {
	return _position;
}
