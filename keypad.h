#include "MKL25Z4.h"

#ifndef KEYPAD_H_
#define KEYPAD_H_

#define R1 (7)
#define R2 (6)
#define R3 (5)
#define R4 (4)

#define C1 (3)
#define C2 (2)
#define C3 (1)
#define C4 (0)

#define MASK(x) (1UL << x)

//void intro();
//void enterMAX();
void activate_keypad();
//void keypad();
//int countnum();

#endif
