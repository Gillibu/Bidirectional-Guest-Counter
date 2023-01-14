/* PTB0 for TRIGGER
 * PTB1 (via potential divider) for ECHO
 *
 */

#include <mkl25z4.h>
#include <stdio.h>
#include <stdint.h>
#include "keypad.h"
#include "lcd.h"
#include "timer.h"
#include "led.h"
#include "stdlib.h"

void init_Timer1();
void init_Timer2();
void init_pin();
void keypad();
void enterMAX();
void intro();
uint16_t countnum();
char* my_itoa(uint16_t number);
void counting();
void numReached();

uint8_t read_distance = 0;
uint8_t read_distance2 = 0;
uint8_t row = 2;
uint8_t column = 1;
uint8_t start =0;

//input capture on rising & falling
uint16_t g_dist=0; //global variable to hold distance.
uint16_t g_dist2=0;
uint8_t counter2 = 0; // increments towards max limit
char max_num[3];
int counter1 = 0; // increments with each key press
uint16_t number;
uint8_t canRead = 0;
uint8_t canRead2 = 0;

int main(){
	LCD_activate_pins();
	init_PWMpins();
	init_Timer1();
	init_Timer2();
	init_LED();
	intro();
	while(1){
	}
}


uint16_t countnum(){    // convert the number in max_num array into an integer
	number = atoi(max_num);
	return number;
}


void intro(){
	char abc[] = "WELCOME TO";
	Screen_display(abc);
	LCD_write_nextln();
	LCD_write_string("GUEST COUNTER");
	delay_ms(3000);
	enterMAX();
}

void enterMAX(){
	memset(max_num,0,3);
	read_distance=0;
	read_distance2=0;
	counter1=0;
	counter2=0;
	start = 0;
	char abc[] = "INPUT THE NUMBER";
	Screen_display(abc);
	LCD_write_nextln();
	LCD_write_string("OF GUESTS");
	delay_ms(2000);
	char bac[] = "NUMBER: ";
	Screen_display(bac);
	delay_ms(200);
	keypad();
}

void keypad(){

	activate_keypad();
	//set up rows to be output
	PTD->PDDR |=  MASK(R1) ;
	PTD->PDDR |=  MASK(R2) ;
	PTD->PDDR |=  MASK(R3) ;
	PTD->PDDR |=  MASK(R4) ;

	//set up columns to be input
	PTD->PDDR &= ~MASK(C1) ;
	PTD->PDDR &= ~MASK(C2) ;
	PTD->PDDR &= ~MASK(C3) ;
	PTD->PDDR &= ~MASK(C4) ;


	while(1) {

		// Take row 1 to low and write the various keypad configurations
		PTD->PDOR &= ~MASK(R1);
		PTD->PDOR |= MASK(R2);
		PTD->PDOR |= MASK(R3);
		PTD->PDOR |= MASK(R4);
		delay_us(2); // a 2 pulse (us) timing delay_ms after setting signal to low

		if((~PTD->PDIR) & MASK(C1)){
			LCD_write_char('1');
			max_num[counter1] = '1';
			counter1++;
			delay_ms(600);
		}

		else if((~PTD->PDIR) & MASK(C2)){
			LCD_write_char('2');
			max_num[counter1] = '2';
			counter1++;
			delay_ms(600);

		}

		else if((~PTD->PDIR) & MASK(C3)){
			LCD_write_char('3');
			max_num[counter1] = '3';
			counter1++;
			delay_ms(600);
		}

		else if((~PTD->PDIR) & MASK(C4)){

		}

		// Take row 2 to low and write the various keypad configurations
		PTD->PDOR |= MASK(R1);
		PTD->PDOR &= ~MASK(R2);
		PTD->PDOR |= MASK(R3);
		PTD->PDOR |= MASK(R4);
		delay_us(2);

		if((~PTD->PDIR) & MASK(C1)){
			LCD_write_char('4');
			max_num[counter1] = '4';
			counter1++;
			delay_ms(600);
		}

		else if((~PTD->PDIR) & MASK(C2)){
			LCD_write_char('5');
			max_num[counter1] = '5';
			counter1++;
			delay_ms(600);
		}

		else if((~PTD->PDIR) & MASK(C3)){
			LCD_write_char('6');
			max_num[counter1] = '6';
			counter1++;
			delay_ms(300);
		}

		else if((~PTD->PDIR) & MASK(C4)){

		}

		// Take row 3 to low and write the various keypad configurations
		PTD->PDOR |= MASK(R1);
		PTD->PDOR |= MASK(R2);
		PTD->PDOR &= ~MASK(R3);
		PTD->PDOR |= MASK(R4);
		delay_us(2);

		if((~PTD->PDIR) & MASK(C1)){
			LCD_write_char('7');
			max_num[counter1] = '7';
			counter1++;
			delay_ms(600);
		}

		else if((~PTD->PDIR) & MASK(C2)){
			LCD_write_char('8');
			max_num[counter1] = '8';
			counter1++;
			delay_ms(600);
		}

		else if((~PTD->PDIR) & MASK(C3)){
			LCD_write_char('9');
			max_num[counter1] = '9';
			counter1++;
			delay_ms(600);
		}

		else if((~PTD->PDIR) & MASK(C4)){

		}

		// Take row 4 to low and write the various keypad configurations
		PTD->PDOR |= MASK(R1);
		PTD->PDOR |= MASK(R2);
		PTD->PDOR |= MASK(R3);
		PTD->PDOR &= ~MASK(R4);
		delay_us(2);

		if((~PTD->PDIR) & MASK(C1)){
			//reset
			delay_ms(600);
			enterMAX();
			read_distance=0;
			read_distance2=0;
		}

		else if((~PTD->PDIR) & MASK(C2)){
			LCD_write_char('0');
			max_num[counter1] = '0';
			counter1++;
			delay_ms(600);
		}

		else if((~PTD->PDIR) & MASK(C3)){
			delay_ms(600);
			read_distance = 1;
			start =1;
		    countnum();
			counting();
			canRead=1;
		}

		else if((~PTD->PDIR) & MASK(C4)){

		}

	}

}


void init_Timer1(){//use channel0 for signal generation, channel 1 for input capture
	//Clock gate
	SIM->SCGC6 |=SIM_SCGC6_TPM1_MASK; //TPM1 channel 0
	//Select clock source in SIM_SOPT //system clock
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1) ;
	//Timeout is is 50ms =8192 or 0x2000
	TPM1->MOD= 4920; //use value -1
	//Channel 0 PWM: MSB-A==10 ELSB-A 10, //PWM output, interrupts not needed.
	TPM1->CONTROLS[0].CnSC |= TPM_CnSC_MSB(1) |TPM_CnSC_ELSB(1) ;
	TPM1->CONTROLS[0].CnV |= 2 ; //For trigger of 20us, CnV=3.2
	//input capture: MSB-A==00 ELSB-A 11; for rising & falling edge
	TPM1->CONTROLS[1].CnSC |= TPM_CnSC_ELSA(1) |TPM_CnSC_ELSB(1) ;
	TPM1->CONTROLS[1].CnSC |= TPM_CnSC_CHF_MASK | TPM_CnSC_CHIE_MASK ; //enable int
	TPM1->SC |= TPM_SC_TOF_MASK | TPM_SC_PS(7) | TPM_SC_TOIE_MASK ;
	TPM1->SC |= TPM_SC_CMOD(1); //enable internal clock to run

	NVIC_ClearPendingIRQ(TPM1_IRQn);
	NVIC_SetPriority(TPM1_IRQn, 3);
	NVIC_EnableIRQ(TPM1_IRQn);
}

void init_Timer2(){//use channel0 for signal generation, channel 1 for input capture
	//Clock gate
	SIM->SCGC6 |=SIM_SCGC6_TPM2_MASK; //TPM2 channel 0
	//Select clock source in SIM_SOPT //system clock
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1) ;
	//Timeout is is 50ms =8192 or 0x2000
	TPM2->MOD= 4920; //use value -1
	//Channel 0 PWM: MSB-A==10 ELSB-A 10, //PWM output, interrupts not needed.
	TPM2->CONTROLS[0].CnSC |= TPM_CnSC_MSB(1) |TPM_CnSC_ELSB(1) ;
	TPM2->CONTROLS[0].CnV |= 2 ; //For trigger of 20us, CnV=3.2
	//input capture: MSB-A==00 ELSB-A 11; for rising & falling edge
	TPM2->CONTROLS[1].CnSC |= TPM_CnSC_ELSA(1) |TPM_CnSC_ELSB(1) ;
	TPM2->CONTROLS[1].CnSC |= TPM_CnSC_CHF_MASK | TPM_CnSC_CHIE_MASK ; //enable int
	TPM2->SC |= TPM_SC_TOF_MASK | TPM_SC_PS(7) | TPM_SC_TOIE_MASK ;
	TPM2->SC |= TPM_SC_CMOD(1); //enable internal clock to run

	NVIC_ClearPendingIRQ(TPM2_IRQn);
	NVIC_SetPriority(TPM2_IRQn, 3);
	NVIC_EnableIRQ(TPM2_IRQn);
}


void TPM1_IRQHandler(){
	static int ctr=0;
	static unsigned int previous=0;
	unsigned int current=0;
	static unsigned int interval=0;
	uint8_t num = countnum();


	if (TPM1->STATUS & TPM_STATUS_CH1F_MASK){ //input capture occured?
		current=TPM1->CONTROLS[1].CnV;
		current |= (ctr <<13);//add no of overflows. Each ctr tick is 2^16
		interval = current-previous;
		previous=current;
		TPM1->CONTROLS[1].CnSC |=TPM_CnSC_CHF_MASK; //clear flag
	}
		if (TPM1->SC & TPM_SC_TOF_MASK){
			if(counter2 < num && start == 1) {
			ctr++; //a timer overflow occurred.
			if (!( ctr %10)){ //check every 10 times
				g_dist=interval; //needed to multiply bt 1.047. Just approx

				if (read_distance == 1){
					if (interval<68 && canRead){
						counter2++;
						PTB->PDOR |= MASK(RED_LED);
						delay_ms(400);
						PTB->PDOR &= ~MASK(RED_LED);
						read_distance2 = 1;
						//printf("discard %d\n", g_dist);
						//uint16_t num = countnum();
						char stor[3]; //create an empty string to store number
						sprintf(stor, "%d", counter2); //make the number into string using sprintf function
						LCD_set_cursor(row,column);
						delay_ms(300);
						LCD_write_string(stor);

						//counter2++;
						canRead = 0;
						//}
					}
					if(interval >= 100) {
						canRead = 1;
					}

					else
						printf("distance in mm=%d\n", g_dist);
				}
			}
		}
	}
//	if(counter2 == num && start == 1) {
//		numReached();
//}
	TPM1->SC |= TPM_SC_TOF_MASK ; //clear the interrupt
}

void TPM2_IRQHandler(){
	static int ctr=0;
	static unsigned int previous=0;
	unsigned int current=0;
	static unsigned int interval=0;
	//uint16_t num = countnum();


	if (TPM2->STATUS & TPM_STATUS_CH1F_MASK){ //input capture occured?
		current=TPM2->CONTROLS[1].CnV;
		current |= (ctr <<13);//add no of overflows. Each ctr tick is 2^16
		interval = current-previous;
		previous=current;
		TPM2->CONTROLS[1].CnSC |=TPM_CnSC_CHF_MASK; //clear flag
	}
	if(counter2 != 0) {
		if (TPM2->SC & TPM_SC_TOF_MASK){
			ctr++; //a timer overflow occurred.
			if (!( ctr %10)){ //check every 10 times
				g_dist2=interval; //needed to multiply bt 1.047. Just approx
				/**
				 * actual calculation:
				 * Clock freq is 20,971,520
				 * PS= 128
				 * => 20.9M/128 -->1s
				 * time for cnv--> cnv/20.9M*128 <=T
				 * dist = speed * time
				 * = 340 * T
				 * = (double distance and in meters)
				 * dist in mm = (340*T)/2*1000
				 * = cnv *1.038 mm
				 * (or 1.047 if using speed as 343m/s)
				 */
				if (read_distance2 == 1){
					if (interval<68 && canRead2){
						counter2--;
						PTB->PDOR |= MASK(GREEN_LED);
						delay_ms(400);
						PTB->PDOR &= ~MASK(GREEN_LED);
						//printf("discard %d\n", g_dist);
						//uint16_t num = countnum();
						char stor[3]; //create an empty string to store number
						sprintf(stor, "%d", counter2); //make the number into string using sprintf function
						LCD_set_cursor(row,column);
						delay_ms(300);
						LCD_write_string(stor);

						//counter2++;
						canRead2 = 0;
						//}
					}
					if(interval >= 100) {
						canRead2 = 1;
					}

					//else
					//printf("distance2 in mm=%d\n", g_dist2);
				}

			}
		}
	}
	TPM2->SC |= TPM_SC_TOF_MASK ; //clear the interrupt
}

char* my_itoa(uint16_t numb) {
	char str[3]; //create an empty string to store number
	sprintf(str, "%d", numb); //make the number into string using sprintf function
	printf("str: %s\n",str);
	return str;
}

void counting(){
	delay_ms(1000);
	char stprt[] = "COUNTING...";
	Screen_display(stprt);
	LCD_write_nextln();
	//LCD_write_string("");
}

void numReached(){
	delay_ms(600);
	char endprt[] ="GUESTS ARRIVED!";
	Screen_display(endprt);
	keypad();
}
