#include "MKL25Z4.h"
#include "keypad.h"

void activate_keypad(){

	SIM->SCGC5|= MASK(12);


	//Set columns to high
	PORTD->PCR[C1] &= ~0x700;
	PORTD->PCR[C1] |= MASK(8);
	PORTD->PCR[C1] |=  0x003;

	PORTD->PCR[C2] &= ~0x700;
	PORTD->PCR[C2] |= MASK(8);

	PORTD->PCR[C3] &= ~0x700;
	PORTD->PCR[C3] |= MASK(8);
	PORTD->PCR[C3] |=  0x003;

	PORTD->PCR[C4] &= ~0x700;
	PORTD->PCR[C4] |= MASK(8);
	PORTD->PCR[C4] |=  0x003;

	//set up rows as GPIO
	PORTD->PCR[R1] &= ~0x700;
	PORTD->PCR[R1] |= MASK(8);

	PORTD->PCR[R2] &= ~0x700;
	PORTD->PCR[R2] |= MASK(8);

	PORTD->PCR[R3] &= ~0x700;
	PORTD->PCR[R3] |= MASK(8);

	PORTD->PCR[R4] &= ~0x700;
	PORTD->PCR[R4] |= MASK(8);
}
