#include "timer.h"
#include "mkl25z4.h"

#define PTE20 (20) //PTB0 associated with TPM1 CH0 //PWM trigger.
#define PTE21 (21) //PTB1 associated with TPM1 CH1
#define PTE22 (22)
#define PTE23 (23)

void init_PWMpins(){
	//set up pin PTB1 for input capture
	SIM->SCGC5 |=SIM_SCGC5_PORTE_MASK;
	PORTE->PCR[PTE21] &=~PORT_PCR_MUX_MASK;
	PORTE->PCR[PTE21] |=PORT_PCR_MUX(3);
	PORTE->PCR[PTE23] &=~PORT_PCR_MUX_MASK;
	PORTE->PCR[PTE23] |=PORT_PCR_MUX(3);

	//set up pin PTB0 for PWM
	SIM->SCGC5 |=SIM_SCGC5_PORTE_MASK;
	PORTE->PCR[PTE20] &=~PORT_PCR_MUX_MASK;
	PORTE->PCR[PTE20] |=PORT_PCR_MUX(3);
	PORTE->PCR[PTE22] &=~PORT_PCR_MUX_MASK;
	PORTE->PCR[PTE22] |=PORT_PCR_MUX(3);
}
