#include "clock.h"
#include "led.h"

int main(void){
	initSysClock();
	initLED();
	while(1){
		GPIOA->BSRR |= (1 <<5);
		delay();
	}
}

