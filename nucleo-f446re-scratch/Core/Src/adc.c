#include "adc.h"


void initGPIOa0(void){
	RCC->AHB1ENR |= (1 << 0); // clock for GPIOA
	GPIOC->MODER |= (3 << 0);
}

void initGPIOa4(void){
	RCC->AHB1ENR |= (1 << 0); // clock for GPIOA
	GPIOC->MODER |= (3 << 8);
}

void initGPIOc1(void){
	RCC->AHB1ENR |= (1 << 2); // clock for GPIOC
	GPIOC->MODER |= (3 << 2);
}

void initADCa0(void){
	RCC->APB2ENR |= (1 << 8); // clock for ADC unit1

}

void initADCa4(void){
	RCC->APB2ENR |= (1 << 8); // clock for ADC unit1
	RCC->AHB1ENR |= (1 << 0); // clock for GPIOA
}

void initADCc1(void){
	RCC->APB2ENR |= (1 << 8); // clock for ADC unit1
	RCC->AHB1ENR |= (1 << 2); // clock for GPIOC
}

init ADXL335(void){
	RCC->AHB1ENR |= (1 << 0); 	// clock for GPIOA
	RCC->AHB1ENR |= (1 << 2); 	// clock for GPIOC
	RCC->APB2ENR |= (1 << 8); 	// clock for ADC unit1
	GPIOC->MODER |= (3 << 0);	// mode for PA0
	GPIOC->MODER |= (3 << 8);	// mode for PA4
	GPIOC->MODER |= (3 << 2);	// mode for PC1

	ADC1->CR1
}
