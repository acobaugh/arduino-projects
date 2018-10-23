/*
 * The idea for this came from:
 *  https://playground.arduino.cc/Main/RotaryEncoders#Example17
 */

#ifndef ENCODER_H

#include <Arduino.h>

class Encoder {
	public:
		Encoder(int8_t pinA, int8_t pinB);
		void readPosition();
		void setPosition(long int position);
		long int getPosition();
	private:
		int _pinA, _pinB;
		int curModeA, curModeB;
		int lastModeA, lastModeB;
		long int _position;
};

#define ENCODER_H
#endif
