#include "MKL25Z4.h"
#include "led.h"

void init_LED(){
	SIM->SCGC5 |=SIM_SCGC5_PORTB_MASK;
	PORTB->PCR[RED_LED] &= ~0X700;	//Clear mux
	PORTB->PCR[RED_LED] |= MASK(8);	//setup to be GPIO
	PORTB->PCR[GREEN_LED] &= ~0X700;	//Clear mux
	PORTB->PCR[GREEN_LED] |= MASK(8);	//setup to be GPIO

	PTB->PDDR |= MASK(RED_LED);
	PTB->PDDR |= MASK(GREEN_LED);
}
