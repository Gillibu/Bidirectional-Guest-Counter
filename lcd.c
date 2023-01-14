#include "MKL25Z4.h"
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "keypad.h"
#include "lcd.h"

#define RS (0)
#define RW (1)
#define E  (2)
#define ROWS 2
#define COLUMNS 16
#define ROW_1 0x80
#define ROW_2 0xC0

void delay_us(uint16_t delay){
	for( int i = 0; i<delay;i++ ){
	__asm volatile ("nop");
	__asm volatile ("nop");
	__asm volatile ("nop");
	__asm volatile ("nop");
	__asm volatile ("nop");
	__asm volatile ("nop");
	__asm volatile ("nop");
	__asm volatile ("nop");
	}
}


void delay_ms(uint16_t Delay){
	for( int j = 0; j< 984*Delay; j++ ){
		__asm volatile ("nop");
		__asm volatile ("nop");
	}
}


void LCD_activate_pins(){
	SIM->SCGC5|= MASK(10);
	SIM->SCGC5|= MASK(11);

	PORTC->PCR[0] &= ~0x700;
	PORTC->PCR[0] |= MASK(8); //D0

	PORTC->PCR[1] &= ~0x700;
	PORTC->PCR[1] |= MASK(8); //D1

	PORTC->PCR[2] &= ~0x700;
	PORTC->PCR[2] |= MASK(8); //D2

	PORTC->PCR[3] &= ~0x700;
	PORTC->PCR[3] |= MASK(8); //D3

	PORTC->PCR[4] &= ~0x700;
	PORTC->PCR[4] |= MASK(8); //D4

	PORTC->PCR[5] &= ~0x700;
	PORTC->PCR[5] |= MASK(8); //D5

	PORTC->PCR[6] &= ~0x700;
	PORTC->PCR[6] |= MASK(8); //D6

	PORTC->PCR[7] &= ~0x700;
	PORTC->PCR[7] |= MASK(8); //D7

	PORTB->PCR[RS] &= ~0x700;
	PORTB->PCR[RS] |= MASK(8); //RS

	PORTB->PCR[RW] &= ~0x700;
	PORTB->PCR[RW] |= MASK(8); //RW

	PORTB->PCR[E] &= ~0x700;
	PORTB->PCR[E] |= MASK(8); //E


	//Setting pins as output
	PTC->PDDR |=  MASK(0);
	PTC->PDDR |=  MASK(1);
	PTC->PDDR |=  MASK(2);
	PTC->PDDR |=  MASK(3);
	PTC->PDDR |=  MASK(4);
	PTC->PDDR |=  MASK(5);
	PTC->PDDR |=  MASK(6);
	PTC->PDDR |=  MASK(7);
	PTB->PDDR |=  MASK(RS);
	PTB->PDDR |=  MASK(RW);
	PTB->PDDR |=  MASK(E);
}

void LCD_command(uint8_t command){
	PTB-> PCOR = MASK(E);
	PTC-> PDOR = command;
	PTB-> PSOR = MASK(E);
	delay_ms(1);

	PTB-> PCOR = MASK(E);
	delay_ms(1);

}

void LCD_set_cursor(uint8_t row, uint8_t column){
	uint8_t command = 0x00;
	// error handling
	if(row > ROWS)
	{
		row = 2;
	}
	if(column > COLUMNS)
	{
		column = 16;
	}
	if(row == 1)
	{
		row = ROW_1;
	}
	else
	{
		row = ROW_2;
	}
	column -= 1;
	command |= (row | column);
	PTB->PDOR &= ~MASK(RS);
	PTB->PDOR &= ~MASK(RW);
	LCD_command(command);
}

void LCD_init(){
	delay_ms(30);

	PTB->PDOR &= ~MASK(RS);
	PTB->PDOR &= ~MASK(RW);

	LCD_command(0x30);

	delay_ms(10);
	LCD_command(0x30);

	delay_us(200);
	LCD_command(0x30);
	LCD_command(0x3C);


	//LCD DisplayOff;
	PTB->PDOR &= ~MASK(RS);
	PTB->PDOR &= ~MASK(RW);
	LCD_command(0X08);

	//Clear Display;
	PTB->PDOR &= ~MASK(RS);
	PTB->PDOR &= ~MASK(RW);
	LCD_command(0x01);

	//LCD DisplayOn;
	PTB->PDOR &= ~MASK(RS);
	PTB->PDOR &= ~MASK(RW);
	LCD_command(0x0F);

	LCD_command(0x06); //Entry mode
}

void Screen_display(char abc[]){
	LCD_init();

	LCD_write_string(abc);
}

void LCD_write_string(char string[]){
	int i=0;
	for(i=0; i<strlen(string); i++){
		PTB->PDOR |= MASK(RS);
		PTB->PDOR &=~MASK(RW);
		LCD_command(string[i]);
	}
}

void LCD_write_char(char letter){

 PTB->PDOR |= (1 << RS);
 PTB->PDOR &= ~(1 << RW);
 LCD_command(letter);

}

void LCD_write_nextln(){

	PTB -> PDOR &= ~MASK(RS);
	PTB -> PDOR &= ~(1 << RW);
	delay_ms(50);
	LCD_command(0xC0);
}
