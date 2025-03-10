#include "main.h"
#include "clock.h"


int main(void){
	configSysClock();
	configGPIO();
	while(1){
		GPIOA->BSRR |= (1 <<5);
		delay();
	}
}

void configGPIO(void){
	RCC->AHB1ENR |= (1 << 0);
	GPIOA->MODER |= (1 << 10); // pin PA5
	GPIOA->OTYPER = 0;
	GPIOA->OSPEEDR = 0;
}
