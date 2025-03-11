#include "led.h"

void initLED(void){
	RCC->AHB1ENR |= (1 << 0);
	GPIOA->MODER |= (1 << 10); // pin PA5
	GPIOA->OTYPER = 0;
	GPIOA->OSPEEDR = 0;
}

void toggleLED(uint8_t t){
	GPIOA->BSRR |= (1 <<5);
	delay(t);
}
