#include "gpio.h"

void initGPIOA(void){
	RCC->AHB1ENR |= (1 << 0);
	GPIOA->MODER |= (3 << 0) | (3 << 2) | (3 << 4);
}

