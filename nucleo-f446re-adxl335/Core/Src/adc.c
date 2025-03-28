#include "adc.h"

void initADC(void){
	RCC->APB2ENR |= (1 << 8);
}

void startADC(void){
	ADC1->CR2 |= (1 << 0);
}

void stopADC(void){
	ADC1->CR2 &= ~(1 << 0);
}

void readADC(){
	ADC1->CR2 |= (1 << 30);
}

