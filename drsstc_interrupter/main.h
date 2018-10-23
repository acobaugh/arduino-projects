#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include "encoder.h"

extern volatile uint8_t& outputPORTx;
extern const int outputBit;

void doFixed();
void doBurst();
void doMIDI();
void resetEncoders();
void readEncoders();

typedef void (* ModeFP)();

#endif
