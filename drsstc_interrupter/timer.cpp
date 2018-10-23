#include <avr/io.h>
#include <avr/interrupt.h>
#include "main.h"

void setupTimer() {
	// Set Timer1 to CTC mode
	TCCR1A |= 0;
	TCCR1B |= (1 << WGM12);
}

void setInterrupter(double ontime, double bps) {
	// do something
}

ISR (TIMER1_COMPA_vect) {
	// set output pin high
	outputPORTx |= (1 << outputBit);
}
