#include "MKL25Z4.h"
#include <stdint.h>
#include <math.h>
#include <string.h>

#ifndef LCD_H_
#define LCD_H_

void LCD_write_nextln();
void LCD_write_char(char letter);
void Screen_display(char abc[]);
void LCD_write_string(char string[]);
void LCD_init();
void LCD_command(uint8_t command);
void LCD_activate_pins();
void delay_us(uint16_t delay);
void delay_ms(uint16_t delay);
void LCD_set_cursor(uint8_t row, uint8_t column);
#endif
