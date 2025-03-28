#include "clock.h"
#include "gpio.h"
#include "adc.h"

void main(void){
	initSysClock();
	initGPIO();
	initADC();
}
