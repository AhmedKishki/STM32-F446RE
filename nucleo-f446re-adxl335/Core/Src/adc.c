#include "adc.h"

void initADC(void){
	RCC->APB2ENR |= (1 << 8);
}
