#include "stm32f446xx.h"

void initUART(void);
void sendChar (uint8_t);
void sendString (char*);
uint8_t getChar (void);
void printADC(float*);
